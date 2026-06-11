/*
 * Author: Vincent Lagerros vinlag@kth.se
 * Date: 2024-04-17
 *
 * Fast sorting for integers of size 8-32 signed and unsigned
 */

use std::mem::swap;
use std::mem::transmute;

impl<T> Sorting<T> for Vec<T> {
    fn sort_fast<P>(&mut self)
    where
        T: SortingPrimative<P>,
    {
        let slice = self.as_mut_slice();
        let hint = <T>::size_hint(slice);
        let mut tmp = Vec::<T>::with_capacity(hint);
        unsafe {
            tmp.set_len(hint);
        }
        if <T>::sort_fast_internal(slice, &mut tmp) {
            swap(self, &mut tmp);
        }
    }

    fn sort_fast_reuse<P>(&mut self, tmp: &mut Vec<T>)
    where
        T: SortingPrimative<P>,
    {
        let slice = self.as_mut_slice();
        let hint = <T>::size_hint(slice);
        tmp.reserve_exact(hint.saturating_sub(tmp.len()));
        unsafe {
            tmp.set_len(hint);
        }
        if <T>::sort_fast_internal(slice, tmp) {
            swap(self, tmp);
        }
    }
}

pub trait Sorting<T> {
    fn sort_fast<P>(&mut self)
    where
        T: SortingPrimative<P>;
    fn sort_fast_reuse<P>(&mut self, tmp: &mut Vec<T>)
    where
        T: SortingPrimative<P>;
}

pub trait SortingPrimative<P>: Sized {
    fn sort_fast_internal(data: &mut [Self], tmp: &mut [Self]) -> bool;
    fn size_hint(data: &mut [Self]) -> usize;
}

impl SortingPrimative<u8> for u8 {
    #[inline]
    fn sort_fast_internal(data: &mut [Self], _: &mut [Self]) -> bool {
        let mut count: [usize; 0x100] = [0; 0x100];
        for &s in data.iter() {
            count[s as usize] += 1;
        }
        let mut sum = 0;
        let mut item = 0;
        for c in count {
            for i in sum..sum + c {
                unsafe {
                    *data.get_unchecked_mut(i) = item;
                }
            }
            sum += c;
            item = item.wrapping_add(1);
        }

        false
    }

    fn size_hint(_: &mut [Self]) -> usize {
        0
    }
}

impl SortingPrimative<i8> for i8 {
    #[inline]
    fn sort_fast_internal(data: &mut [Self], _: &mut [Self]) -> bool {
        let mut count: [usize; 0x100] = [0; 0x100];
        for &s in data.iter() {
            count[(i8::cast_unsigned(s) ^ 0b10000000) as usize] += 1;
        }
        let mut sum = 0;
        let mut item = i8::MIN;
        for c in count {
            for i in sum..sum + c {
                unsafe {
                    *data.get_unchecked_mut(i) = item;
                }
            }
            sum += c;
            item = item.wrapping_add(1);
        }
        false
    }

    fn size_hint(_: &mut [Self]) -> usize {
        0
    }
}

const SIZE_CUTOFF: usize = 100;

macro_rules! impl_radix {
    ($SortTy:ty, $LabelTy:ty, $name: ident, $swaps:expr; $( $InputTy:ty ),* ) => {
        fn $name(data: &mut [$SortTy], tmp: &mut [$SortTy]) -> bool {
            if tmp.len() != data.len() {
                // fallback algo
                data.sort_unstable();
                return false;
            }

            assert_eq!(data.len(), tmp.len());

            #[inline]
            unsafe fn msd_radix_sort<const OFFSET: usize>(
                source: &[$SortTy],
                dest: &mut [$SortTy],
            ) {
                #[inline]
                const fn get_byte_at<const OFFSET: usize>(input: $SortTy) -> usize {
                    ((input >> OFFSET) & 0xff) as usize
                }

                const LENGTH: usize = 0x100;
                let mut count: [usize; LENGTH] = [0; LENGTH];

                for &s in source {
                    count[get_byte_at::<OFFSET>(s)] += 1;
                }

                for i in 1..LENGTH {
                    count[i] += count[i - 1];
                }

                for &s in source.iter().rev() {
                    let byte_index = get_byte_at::<OFFSET>(s);
                    count[byte_index] -= 1;
                    dest[count[byte_index] as usize] = s;
                }
            }

            fn radix_sort(source: &mut [$SortTy], tmp: &mut [$SortTy]) -> bool {
                if source.len() <= 1 {
                    return false;
                }

                // do the radix sort in steps swapping the pointers
                unsafe {
                    assert!($swaps <= 8);
                    if $swaps >= 1 {
                        msd_radix_sort::<0>(source, tmp);
                    }
                    if $swaps >= 2 {
                        msd_radix_sort::<8>(tmp, source);
                    }
                    if $swaps >= 3 {
                        msd_radix_sort::<16>(source, tmp);
                    }
                    if $swaps >= 4 {
                        msd_radix_sort::<24>(tmp, source);
                    }
                    if $swaps >= 5 {
                        msd_radix_sort::<32>(source, tmp);
                    }
                    if $swaps >= 6 {
                        msd_radix_sort::<40>(tmp, source);
                    }
                    if $swaps >= 7 {
                        msd_radix_sort::<48>(source, tmp);
                    }
                    if $swaps >= 8 {
                        msd_radix_sort::<56>(tmp, source);
                    }
                }

                // if not negative then just return
                use std::any::*;
                if !(TypeId::of::<$SortTy>() == TypeId::of::<i32>()
                    || TypeId::of::<$SortTy>() == TypeId::of::<i64>()
                    || TypeId::of::<$SortTy>() == TypeId::of::<i16>()
                    || TypeId::of::<$SortTy>() == TypeId::of::<i8>()
                    || TypeId::of::<$SortTy>() == TypeId::of::<isize>())
                {
                    return $swaps % 2 == 1;
                }

                let edit = if $swaps % 2 == 1 { tmp } else { source };

                // binary search for the negative top part
                let mut left = 0usize;
                let mut right = edit.len();
                #[allow(unused_comparisons)]
                while left < right {
                    let mid = (left + right) >> 1;
                    if edit[mid] < 0 {
                        right = mid;
                    } else {
                        left = mid + 1;
                    }
                }
                edit.rotate_left(right);
                return $swaps % 2 == 1;
            }
            radix_sort(data, tmp)
        }

        $(impl SortingPrimative<$LabelTy> for $InputTy {
            fn sort_fast_internal(data: &mut [Self], tmp: &mut [Self]) -> bool {
                unsafe { $name(transmute(data), transmute(tmp)) }
            }

            fn size_hint(data: &mut [Self]) -> usize {
                if data.len() > SIZE_CUTOFF {
                    data.len()
                } else {
                    0
                }
            }
        })*
    };
}

#[allow(non_camel_case_types)]
#[repr(transparent)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct i24(i32);

#[repr(transparent)]
#[allow(non_camel_case_types)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct u24(u32);

// transmuted into, sorting type, function name, [from type]
impl_radix!(u32, u32, sort_fast_internal_u32, 4; u32, i32, u24, i24);
impl_radix!(i32, i32, sort_fast_internal_i32, 4; u32, i32, u24, i24);

impl_radix!(u32, u24, sort_fast_internal_u24, 3; u32, i32, u24, i24);
impl_radix!(i32, i24, sort_fast_internal_i24, 3; u32, i32, u24, i24);

impl_radix!(u32, u16, sort_fast_internal_u16_as_u32, 2; u32, i32, u24, i24);
impl_radix!(i32, i16, sort_fast_internal_i16_as_i32, 2; u32, i32, u24, i24);

impl_radix!(u16, u16, sort_fast_internal_u16, 2; u16, i16);
impl_radix!(i16, i16, sort_fast_internal_i16, 2; u16, i16);

//impl_radix!(u64, u64, sort_fast_internal_u64, 6; u64, i64); // this is not working due to SIZE_CUTOFF > 0
// this is radix 8, as radix 16 is slower