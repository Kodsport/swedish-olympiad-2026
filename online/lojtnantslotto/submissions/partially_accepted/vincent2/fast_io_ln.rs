/*
 * Author: Vincent Lagerros vinlag@kth.se
 * Date: 2024-02-21
 *
 * Fast and self-contained unsafe IO for kattis.
 * This is optmized for speed, and can be *very* unsafe if you do not do the input correctly.
 *
 * You have to declare when newlines are used to avoid crashing on windows due to \r\n.
 */
#![allow(unused, non_camel_case_types)]

use std::{mem::transmute, num::NonZeroU8, ops::BitXor};

/// Mainly captures A-Z a-z, but may catch other characters.
///
/// Note this may not work well with FxHasher, so try the default hasher
///
/// ```@, A-Z, [, \, ], ^, _, `, a-z, {, |, }, ~```
#[repr(transparent)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
pub struct AlphaHash(pub u64);

/// Sometimes `&[u8]` is too big
#[derive(Debug, Clone, Copy)]
pub struct InputSliceU32 {
    offset: u32,
    len: u32,
}

impl InputSliceU32 {
    pub fn new(offset: u32, len: u32) -> Self {
        Self {
            offset: offset,
            len: len,
        }
    }
    fn offset(&self) -> u32 {
        self.offset
    }
    fn len(&self) -> u32 {
        self.len
    }
}

// 4 upper bits = len
// 28 lower bits = offset
const INPUT_SLICE_PACKED_OFFSET_BITS: usize = 28;

#[derive(Debug, Clone, Copy)]
pub struct InputSlicePacked {
    data: u32,
}

impl InputSlicePacked {
    pub fn new(offset: u32, len: u32) -> Self {
        Self {
            data: offset | (len << INPUT_SLICE_PACKED_OFFSET_BITS),
        }
    }
    fn offset(&self) -> u32 {
        self.data & ((1 << INPUT_SLICE_PACKED_OFFSET_BITS) - 1)
    }
    fn len(&self) -> u32 {
        self.data >> INPUT_SLICE_PACKED_OFFSET_BITS
    }
}

pub type InputSlice = InputSliceU32;

/// Mainly captures A-Z a-z 0-9, but may catch other characters.
///
/// Note this may not work well with FxHasher, so try the default hasher
///
/// ```0-9, :, ;, <, =, >, ?, @, A-Z, [, \, ], ^, _, `, a-z, {, |, }, ~```
#[repr(transparent)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
pub struct AlphaNumericalHash(pub u64);

#[repr(transparent)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
pub struct NewlineHash(pub u64);

/// The max amount of chars a Binary can consume
type Move = u32;

#[cfg(not(target_os = "windows"))]
pub const NEWLINE_LEN: usize = 1;
#[cfg(target_os = "windows")]
pub const NEWLINE_LEN: usize = 2;

#[cfg(not(target_os = "windows"))]
pub const NEWLINE_CHAR: u8 = b'\n';
#[cfg(target_os = "windows")]
pub const NEWLINE_CHAR: u8 = b'\r';

pub struct Io<'a> {
    /// Position into the stdin buffer, can be out of bounds
    pos: usize,
    /// Entire stdin buffer
    input: &'a [u8],
    /// Delayed bytearray to send to stdout for batch write
    output: Vec<u8>,
}

pub trait Binary<'a> {
    fn take(data: &'a [u8]) -> (Self, Move)
    where
        Self: Sized;

    #[inline]
    fn take_impl<const ADD: usize>(io: &mut Io<'a>) -> Self
    where
        Self: Sized,
    {
        let (out, len) = Self::take(&io.input[io.pos..]);
        io.pos += len as usize + ADD;
        out
    }
}

pub trait FastBinary<const N: usize> {
    fn take_fast(ptr: *const u8) -> (Self, u8)
    where
        Self: Sized;

    #[inline]
    fn take_fast_impl<const ADD: usize>(io: &mut Io<'_>) -> Self
    where
        Self: Sized,
    {
        let (out, len) = Self::take_fast(unsafe { io.input_ptr() });
        io.pos += len as usize + ADD;
        out
    }
}

pub trait FastFloat<const N: usize, const M: usize, const NEGATIVE: bool> {
    fn take_float(ptr: *const u8) -> (Self, u8)
    where
        Self: Sized;
}

/// Pretty formated
pub trait Writable {
    fn write(self, into: &mut Vec<u8>);
    #[inline]
    fn write_impl(self, io: &mut Io<'_>)
    where
        Self: Sized,
    {
        Self::write(self, &mut io.output);
    }
}

impl<'a> Binary<'a> for InputSlice {
    fn take(data: &'a [u8]) -> (Self, Move)
    where
        Self: Sized,
    {
        unreachable!()
    }
    fn take_impl<const ADD: usize>(io: &mut Io<'a>) -> Self
    where
        Self: Sized,
    {
        let len = len_alphanumerical::<{ usize::MAX }>(unsafe { io.input_ptr() });
        let out = InputSlice::new(io.pos as u32, len as u32);
        io.move_input(len + 1);
        out
    }
}

impl<const N: usize> FastBinary<N> for InputSlice {
    fn take_fast(ptr: *const u8) -> (Self, u8)
    where
        Self: Sized,
    {
        unreachable!()
    }
    fn take_fast_impl<const ADD: usize>(io: &mut Io<'_>) -> Self
    where
        Self: Sized,
    {
        let len = len_alphanumerical::<N>(unsafe { io.input_ptr() });
        let out = InputSlice::new(io.pos as u32, len as u32);
        io.pos += len + ADD;
        out
    }
}

impl Writable for InputSlice {
    fn write(self, into: &mut Vec<u8>) {
        unreachable!()
    }
    fn write_impl(self, io: &mut Io<'_>)
    where
        Self: Sized,
    {
        io.output.extend_from_slice(
            &io.input[self.offset() as usize..(self.offset() + self.len()) as usize],
        );
    }
}

/// Fast formated where N indicates how many characters we want
pub trait FastWritable<const N: usize> {
    fn fast_write(self, into: &mut Vec<u8>);
}

impl<'a> Clone for Io<'a> {
    /// Clone without cloning output for a fast copy
    fn clone(&self) -> Self {
        Self {
            pos: self.pos,
            input: self.input,
            output: vec![],
        }
    }
}

impl<'a> Io<'a> {
    /// Takes a string ref as the constructor, can be paired with include_str!()
    pub const fn new(text: &'a str) -> Self {
        Self {
            pos: 0,
            input: text.as_bytes(),
            output: Vec::new(),
        }
    }

    /// Creates an output only Io instance
    pub const fn empty() -> Self {
        Self {
            pos: 0,
            input: b"",
            output: Vec::new(),
        }
    }

    /// Gets the input pointer
    #[inline]
    pub unsafe fn input_ptr(&self) -> *const u8 {
        unsafe { self.input.as_ptr().add(self.pos) }
    }

    /// Moves the input cursor by the set amount
    #[inline]
    pub fn move_input(&mut self, delta: usize) {
        self.pos += delta;
    }

    /// Clears the output, this is useful if you print while you process
    #[inline]
    pub fn clear(&mut self) {
        self.output.clear();
    }

    /// Manually flushes to the stdout, this *can* be faster, but not always
    #[inline]
    pub fn flush(&mut self) {
        sys::write_to_end(&self.output);
        self.output.clear();
    }

    /// Gets the current input position, do not use for anything else than debugging
    #[cfg(target_os = "windows")]
    pub fn dbg(&self, size: usize) -> &str {
        std::str::from_utf8(&self.input[self.pos..(self.pos + size).min(self.input.len())])
            .expect("Valid utf8")
    }

    /// Removes and returns the entire current output buffer, this can be used for testing without printing on drop
    ///
    /// Use `clear` instead if you simply want to clear it
    #[must_use]
    pub fn take_output(&mut self) -> Vec<u8> {
        let mut out = vec![];
        std::mem::swap(&mut self.output, &mut out);
        out
    }

    #[must_use]
    pub fn output(&self) -> &[u8] {
        &self.output
    }

    /// Manually flushes to the stdout buffer if too large, this *can* be faster, but not always
    /// Use this instead of `flush` to guarantee that write syscalls are not wasted
    ///
    /// `0x2000-0x4000` is the optimal range for this, and was found by empirically testing kattis
    #[inline]
    pub fn maybe_flush(&mut self) {
        if self.output.len() > 0x4000 {
            self.flush();
        }
    }

    pub fn output_hint(&mut self, len: usize) {
        self.output
            .reserve_exact(self.output.len().saturating_sub(len));
    }

    /// Malformed but slightly faster output without write!
    #[inline]
    pub fn write_fast<const N: usize>(&mut self, content: impl FastWritable<N>) {
        content.fast_write(&mut self.output);
    }

    /// Pretty formated output without write!
    #[inline]
    pub fn write(&mut self, content: impl Writable) {
        content.write_impl(self);
    }

    /// Writes a bytearray with newline as a &str for easier and faster writing
    #[inline]
    pub fn writeln(&mut self, content: impl Writable) {
        self.write(content);
        self.output.push(b'\n');
    }

    /// Writes a bytearray with space as a &str for easier and faster writing
    #[inline]
    pub fn writesp(&mut self, content: impl Writable) {
        self.write(content);
        self.output.push(b' ');
    }

    /// Writes a single byte, compared to .write that writes the base 10
    #[inline]
    pub fn write_byte(&mut self, content: u8) {
        self.output.push(content);
    }

    /// Takes the stdin as the constructor to read from and returns a static ref
    pub fn stdin() -> Self {
        /*let mut text = Vec::new();
        read_to_end(&mut text);
        let out = Self {
            pos: 0,
            input: unsafe { std::mem::transmute(&text as &[u8]) },
            output: Vec::new(),
        };
        Box::new(text).leak::<'static>(); // known mem leak
        out*/
        Self {
            pos: 0,
            input: sys::mmap_stdin(),
            output: Vec::new(),
        }
    }

    /// Skips over all whitespace at the current location
    pub fn whitespace(&mut self) {
        self.pos += self
            .input
            .iter()
            .skip(self.pos)
            .take_while(|x| x.is_ascii_whitespace())
            .count();
    }

    /// Consumes an entire line, not to be confused with `nextln`
    pub fn next_line(&mut self) -> &'a str {
        #[cfg(target_os = "windows")]
        {
            let len = self
                .input
                .iter()
                .skip(self.pos)
                .take_while(|x| **x != b'\n')
                .count();
            let end = self.pos + len - 1; // \r
            let out = unsafe { std::str::from_utf8_unchecked(&self.input[self.pos..end]) };
            self.pos += len + 1;
            out
        }
        #[cfg(not(target_os = "windows"))]
        {
            let len = len_to_newline(unsafe { self.input_ptr() });
            let end = self.pos + len;
            let out = unsafe { std::str::from_utf8_unchecked(&self.input[self.pos..end]) };
            self.pos += len + 1;
            out
        }
    }

    /// Consumes an entire line, but checks first if we have not reached the EOF
    pub fn next_line_checked(&mut self) -> Option<&'a str> {
        if self.pos >= self.input.len() {
            return None;
        }
        Some(self.next_line())
    }

    /// Takes `size` amount of bytes and moves the cursor with the same size
    pub fn take(&mut self, size: usize) -> &'a [u8] {
        let out = unsafe { std::slice::from_raw_parts(self.input_ptr(), size) };
        self.pos += size;
        out
    }

    /// Takes `size` amount of bytes and moves the cursor with the same size
    pub fn take_mut(&mut self, size: usize) -> &'a mut [u8] {
        let out = unsafe { std::slice::from_raw_parts_mut(self.input_ptr() as *mut u8, size) };
        self.pos += size;
        out
    }

    /// Takes `N` amount of bytes and moves the cursor with the same size
    pub fn taken<const N: usize>(&mut self) -> [u8; N] {
        unsafe {
            let ptr = self.input.as_ptr().add(self.pos);
            self.pos += N;
            *(ptr as *const [u8; N])
        }
    }

    /// Returns if we have reached the end of the file
    pub fn eof(&self) -> bool {
        self.pos >= self.input.len()
    }

    /// Returns true if we just passed to a new line, *and* moves to the next line
    pub fn eol(&mut self) -> bool {
        if self.input.get(self.pos.wrapping_sub(1)) == Some(&NEWLINE_CHAR) {
            self.ln();
            true
        } else {
            false
        }
    }

    /// Parses the next whitespace seperated text as T, but checks first if we have not reached the EOF
    pub fn next_checked<T: Binary<'a>>(&mut self) -> Option<T> {
        if self.pos >= self.input.len() {
            return None;
        }
        Some(self.next())
    }

    /// Parses the next whitespace seperated text as T as the end of a line, but checks first if we have not reached the EOF,
    pub fn next_checkedln<T: Binary<'a>>(&mut self) -> Option<T> {
        if self.pos >= self.input.len() {
            return None;
        }
        Some(self.nextln())
    }

    /// Parses the next whitespace seperated text as T without any checks, when at the end of a line
    pub fn nextln<T: Binary<'a>>(&mut self) -> T {
        T::take_impl::<NEWLINE_LEN>(self)
    }

    /// Parses the next whitespace seperated text as T without any checks
    pub fn next<T: Binary<'a>>(&mut self) -> T {
        T::take_impl::<1>(self)
    }

    /// Parses the next whitespace seperated text as T without any checks
    pub fn next_fast<const N: usize, T: FastBinary<N>>(&mut self) -> T {
        T::take_fast_impl::<1>(self)
    }

    /// Parses the next whitespace seperated text as T without any checks
    pub fn nextln_fast<const N: usize, T: FastBinary<N>>(&mut self) -> T {
        T::take_fast_impl::<NEWLINE_LEN>(self)
    }

    /// Parses the next whitespace seperated text as T without any checks
    ///
    /// N = Prefix length -(123).345
    ///
    /// M = Suffix length -123.(345)
    ///
    /// S = If the output can be negative (-)123.345
    pub fn next_float<const N: usize, const M: usize, const S: bool, T: FastFloat<N, M, S>>(
        &mut self,
    ) -> T {
        let (out, append) = T::take_float(unsafe { self.input_ptr() });
        self.pos += append as usize + 1;
        out
    }

    /// Parses the next whitespace seperated text as T without any checks
    ///
    /// N = Prefix length -(123).345
    ///
    /// M = Suffix length -123.(345)
    ///
    /// S = If the output can be negative (-)123.345
    pub fn nextln_float<const N: usize, const M: usize, const S: bool, T: FastFloat<N, M, S>>(
        &mut self,
    ) -> T {
        let (out, append) = T::take_float(unsafe { self.input_ptr() });
        self.pos += append as usize + NEWLINE_LEN;
        out
    }

    /// unsafe { *self.input_ptr() }
    pub fn peek_char(&self) -> u8 {
        unsafe { *self.input_ptr() }
    }

    pub fn to_str(&self, slice: InputSlice) -> &'a [u8] {
        &self.input[slice.offset() as usize..(slice.offset() + slice.len()) as usize]
    }

    /// Parses the next whitespace seperated text as T without any checks, but does not move the cursor
    /// this does not store the result in any way, so use carefully to avoid performance degradation
    pub fn peek<T: Binary<'a>>(&mut self) -> T {
        let (out, _) = T::take(&self.input[self.pos..]);
        out
    }

    /// Takes a single character and moves the cursor twice,
    /// useful for text like `a 123 b c 42`
    pub fn next_char(&mut self) -> u8 {
        let out = self.input[self.pos];
        self.pos += 2;
        out
    }

    /// Takes a single character and moves the cursor twice at the end of a line,
    /// useful for text like `42 c`
    pub fn next_charln(&mut self) -> u8 {
        let out = self.input[self.pos];
        self.pos += NEWLINE_LEN + 1;
        out
    }

    /// Used in case `nextln` was not called at the end of a line, such as in cases like
    /// ```text
    /// (0..n).map(|_| io.next()).collect::<T>();
    /// io.ln();
    /// ```
    #[inline]
    pub fn ln(&mut self) {
        self.pos += NEWLINE_LEN - 1;
    }

    /// Used for blank lines
    #[inline]
    pub fn blank(&mut self) {
        self.pos += NEWLINE_LEN;
    }
}

impl<const N: usize> Writable for &[u8; N] {
    fn write(self, into: &mut Vec<u8>) {
        debug_assert!(self.is_ascii());
        into.extend_from_slice(self);
    }
}

impl Writable for &[u8] {
    fn write(self, into: &mut Vec<u8>) {
        debug_assert!(self.is_ascii());
        into.extend_from_slice(self);
    }
}

impl Writable for &mut [u8] {
    fn write(self, into: &mut Vec<u8>) {
        debug_assert!(self.is_ascii());
        into.extend_from_slice(self);
    }
}

impl Writable for &str {
    fn write(self, into: &mut Vec<u8>) {
        into.extend_from_slice(self.as_bytes());
    }
}

impl Writable for char {
    fn write(self, into: &mut Vec<u8>) {
        debug_assert!(self.is_ascii());
        into.push(self as u8);
    }
}

macro_rules! impl_write {
    ($unsigned:ty, $signed:ty, $fn:ident, $table:ident) => {
        // https://lemire.me/blog/2021/05/28/computing-the-number-of-digits-of-an-integer-quickly/#comment-585476
        // This is faster than ilog10 and also not rounded down
        pub(crate) const fn $fn(mut x: $unsigned) -> u32 {
            x |= 1;
            let zeros = <$unsigned>::BITS - (x).leading_zeros();
            let len = (1233 * zeros) >> 12;
            len + (x > $table[len as usize] as $unsigned) as u32
        }

        impl Writable for $unsigned {
            fn write(mut self, into: &mut Vec<u8>) {
                let add = $fn(self) as usize;
                into.reserve(add);
                unsafe {
                    into.set_len(into.len() + add);
                }
                let mut i = unsafe { into.as_mut_ptr().add(into.len() - 1) };
                loop {
                    let char = self % 10;
                    self /= 10;
                    unsafe { *i = char as u8 + b'0' };
                    i = unsafe { i.sub(1) };
                    if self == 0 {
                        break;
                    }
                }
            }
        }
        impl Writable for $signed {
            fn write(self, into: &mut Vec<u8>) {
                let mut x = self.unsigned_abs();
                let neg = self < 0;

                let add = $fn(x) as usize + neg as usize;
                into.reserve(add);
                unsafe {
                    into.set_len(into.len() + add);
                }
                let mut i = unsafe { into.as_mut_ptr().add(into.len() - 1) };

                loop {
                    let char = x % 10;
                    x /= 10;
                    unsafe { *i = char as u8 + b'0' };
                    i = unsafe { i.sub(1) };
                    if x == 0 {
                        if neg {
                            unsafe { *i = b'-' };
                        }
                        break;
                    }
                }
            }
        }

        impl<const N: usize> FastWritable<N> for $unsigned {
            fn fast_write(self, into: &mut Vec<u8>) {
                into.resize(into.len() + N, b' ');
                let mut x = self;
                debug_assert!(
                    ($fn(x) as usize) < N,
                    "Too small N for fast_write, expected N={} where N={N}",
                    $fn(x) + 1
                );
                let mut i = unsafe { into.as_mut_ptr().add(into.len() - 1) };

                loop {
                    let char = x % 10;
                    x /= 10;
                    unsafe { *i = char as u8 + b'0' };
                    i = unsafe { i.sub(1) };
                    if x == 0 {
                        break;
                    }
                }
            }
        }

        impl<const N: usize> FastWritable<N> for $signed {
            fn fast_write(self, into: &mut Vec<u8>) {
                into.resize(into.len() + N, b' ');
                let mut x = self.unsigned_abs();
                debug_assert!(
                    ($fn(x) as usize) + ((self < 0) as usize) < N,
                    "Too small N for fast_write, expected N={} where N={N}",
                    $fn(x) + 1 + ((self < 0) as u32)
                );
                let mut i = unsafe { into.as_mut_ptr().add(into.len() - 1) };

                loop {
                    let char = x % 10;
                    x /= 10;
                    unsafe { *i = char as u8 + b'0' };
                    i = unsafe { i.sub(1) };
                    if x == 0 {
                        if self < 0 {
                            unsafe { *i = b'-' };
                        }
                        break;
                    }
                }
            }
        }
    };
}

const TABLE_16: [u16; 6] = [0, 9, 99, 999, 9999, u16::MAX];
const TABLE_32: [u32; 11] = [
    0,
    9,
    99,
    999,
    9999,
    99999,
    999999,
    9999999,
    99999999,
    999999999,
    u32::MAX,
];
const TABLE_64: [u64; 21] = [
    0,
    9,
    99,
    999,
    9999,
    99999,
    999999,
    9999999,
    99999999,
    999999999,
    9999999999,
    99999999999,
    999999999999,
    9999999999999,
    99999999999999,
    999999999999999,
    9999999999999999,
    99999999999999999,
    999999999999999999,
    9999999999999999999,
    u64::MAX,
];
const TABLE_F32_DIV: [f32; 17] = [
    1.0,
    0.1,
    0.01,
    0.001,
    0.0001,
    0.00001,
    0.000001,
    0.0000001,
    0.00000001,
    0.000000001,
    0.0000000001,
    0.00000000001,
    0.000000000001,
    0.0000000000001,
    0.00000000000001,
    0.000000000000001,
    0.0000000000000001,
];
const TABLE_F64_DIV: [f64; 17] = [
    1.0,
    0.1,
    0.01,
    0.001,
    0.0001,
    0.00001,
    0.000001,
    0.0000001,
    0.00000001,
    0.000000001,
    0.0000000001,
    0.00000000001,
    0.000000000001,
    0.0000000000001,
    0.00000000000001,
    0.000000000000001,
    0.0000000000000001,
];

const TABLE_F32_MUL: [f32; 19] = [
    1.0,
    10.0,
    100.0,
    1000.0,
    10000.0,
    100000.0,
    1000000.0,
    10000000.0,
    100000000.0,
    1000000000.0,
    10000000000.0,
    100000000000.0,
    1000000000000.0,
    10000000000000.0,
    100000000000000.0,
    1000000000000000.0,
    10000000000000000.0,
    100000000000000000.0,
    1000000000000000000.0,
];
const TABLE_F64_MUL: [f64; 19] = [
    1.0,
    10.0,
    100.0,
    1000.0,
    10000.0,
    100000.0,
    1000000.0,
    10000000.0,
    100000000.0,
    1000000000.0,
    10000000000.0,
    100000000000.0,
    1000000000000.0,
    10000000000000.0,
    100000000000000.0,
    1000000000000000.0,
    10000000000000000.0,
    100000000000000000.0,
    1000000000000000000.0,
];

impl Writable for u8 {
    fn write(self, into: &mut Vec<u8>) {
        if self < 10 {
            into.push(self + b'0');
        } else if self < 100 {
            into.push((self / 10) + b'0');
            into.push((self % 10) + b'0');
        } else {
            into.push((self / 100) + b'0');
            into.push(((self % 100) / 10) + b'0');
            into.push((self % 10) + b'0');
        }
    }
}

impl_write!(u16, i16, digit_count_16, TABLE_16);
impl_write!(u32, i32, digit_count_32, TABLE_32);
impl_write!(u64, i64, digit_count_64, TABLE_64);
#[cfg(target_pointer_width = "64")]
impl_write!(usize, isize, digit_count_usize, TABLE_64);
#[cfg(target_pointer_width = "32")]
impl_write!(usize, isize, digit_count_usize, TABLE_32);

/// Parses up to 4 digits of a base 10 number, and return is and the length
#[inline]
pub fn parse_1e_n<const N: usize>(data: *const u8) -> (u16, u8) {
    // Branching like this is somehow faster than a non branching solution like parse_1e8?
    // See `shatteredcake` for more details, but I guess many very small numbers make it faster than trailing_zeros
    let x = unsafe { *data };
    let mut out = (x - b'0') as u16;

    for i in 1..N {
        let x = unsafe { *data.add(i) };
        let Some(n) = x.checked_sub(b'0') else {
            debug_assert!(out <= TABLE_16[N]);
            unsafe { std::hint::assert_unchecked(out <= TABLE_16[N]) };
            return (out, i as u8);
        };
        debug_assert!(n <= 9, "unexpected char \'{}\' in parse integer", x as char);

        out = out * 10 + n as u16;
    }

    debug_assert!(out <= TABLE_16[N]);
    unsafe { std::hint::assert_unchecked(out <= TABLE_16[N]) };
    (out, N as u8)
}

#[inline]
pub fn parse_1e_n_u32<const N: usize>(data: *const u8) -> (u32, u8) {
    // Branching like this is somehow faster than a non branching solution like parse_1e8?
    // See `shatteredcake` for more details, but I guess many very small numbers make it faster than trailing_zeros
    let x = unsafe { *data };
    let mut out = (x - b'0') as u32;

    for i in 1..N {
        let x = unsafe { *data.add(i) };
        let Some(n) = x.checked_sub(b'0') else {
            debug_assert!(out <= TABLE_32[N]);
            unsafe { std::hint::assert_unchecked(out <= TABLE_32[N]) };
            return (out, i as u8);
        };
        debug_assert!(n <= 9, "unexpected char \'{}\' in parse integer", x as char);

        out = out * 10 + n as u32;
    }

    debug_assert!(out <= TABLE_32[N]);
    unsafe { std::hint::assert_unchecked(out <= TABLE_32[N]) };
    (out, N as u8)
}

#[inline(always)]
pub fn parse_1e1(data: *const u8) -> (u16, u8) {
    parse_1e_n::<1>(data)
}

#[inline(always)]
pub fn parse_1e2(data: *const u8) -> (u16, u8) {
    parse_1e_n::<2>(data)
}

#[inline(always)]
pub fn parse_1e3(data: *const u8) -> (u16, u8) {
    parse_1e_n::<3>(data)
}

#[inline(always)]
pub fn parse_1e4(data: *const u8) -> (u16, u8) {
    parse_1e_n::<4>(data)
}

#[inline(always)]
pub fn parse_1e5(data: *const u8) -> (u32, u8) {
    parse_1e_n_u32::<5>(data)
}

#[inline(always)]
pub fn parse_1e6(data: *const u8) -> (u32, u8) {
    parse_1e_n_u32::<6>(data)
}

/// Parses up to 8 digits of a base 10 number, and return is and the length
/// ~32 instructions
#[inline]
pub fn parse_1e8(data: *const u8) -> (u32, u8) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) }; // max 8 chars

    let zero_to_nine = chunk ^ 0x3030303030303030;
    let non_zero = zero_to_nine & 0xF0F0F0F0F0F0F0F0;
    let len = non_zero.trailing_zeros() as u8 >> 3;

    // https://lemire.me/blog/2022/01/21/swar-explained-parsing-eight-digits/
    let chunk = zero_to_nine << (8 * (8 - len));
    const MASK: u64 = 0x000000FF000000FF;
    const MUL1: u64 = 0x000F424000000064; // 100 + (1000000ULL << 32)
    const MUL2: u64 = 0x0000271000000001; // 1 + (10000ULL << 32)
    let chunk = (chunk.wrapping_mul(10)).wrapping_add(chunk >> 8); // val = (val * 2561) >> 8;
    let chunk = (((chunk & MASK).wrapping_mul(MUL1))
        .wrapping_add(((chunk >> 16) & MASK).wrapping_mul(MUL2)))
        >> 32;
    let out = chunk as u32;
    debug_assert!(out <= 9999_9999);
    unsafe { std::hint::assert_unchecked(out <= 9999_9999) };
    (out as u32, len)
}

/// Parses up to 16 digits of a base 10 number, and return is and the length
/// ~64 instructions
#[inline]
pub fn parse_1e16(data: *const u8) -> (u64, u8) {
    /*
    SIMD is slower by a lot

    use std::arch::x86_64::{
        _mm_cvtsi128_si64, _mm_lddqu_si128, _mm_madd_epi16, _mm_maddubs_epi16, _mm_packus_epi32,
        _mm_set1_epi8, _mm_set_epi16, _mm_set_epi8, _mm_sub_epi16,
    };

    let out = unsafe {
        let chunk = _mm_loadu_epi64(std::mem::transmute(&chunk));

        let mult = _mm_set_epi8(1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10);
        let chunk = _mm_maddubs_epi16(chunk, mult);

        let mult = _mm_set_epi16(1, 100, 1, 100, 1, 100, 1, 100);
        let chunk = _mm_madd_epi16(chunk, mult);

        let chunk = _mm_packus_epi32(chunk, chunk);
        let mult = _mm_set_epi16(0, 0, 0, 0, 1, 10000, 1, 10000);
        let chunk = _mm_madd_epi16(chunk, mult);

        let ra = _mm_extract_epi64(chunk, 0) as u64;

        ((ra & 0xffffffff) * 100000000) + (ra >> 32)
    };
    */

    let chunk = unsafe { std::ptr::read_unaligned(data as *const u128) }; // max 16 chars

    let zero_to_nine = chunk ^ 0x30303030303030303030303030303030;
    let non_zero = zero_to_nine & 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0;
    let len = non_zero.trailing_zeros() as u8 >> 3;
    let chunk = zero_to_nine << (8 * (16 - len));

    let lower_digits = (chunk & 0x0f000f000f000f000f000f000f000f00) >> 8;
    let upper_digits = (chunk & 0x000f000f000f000f000f000f000f000f) * 10;
    let chunk = lower_digits + upper_digits;

    // 2-byte mask trick (works on 4 pairs of two digits)
    let lower_digits = (chunk & 0x00ff000000ff000000ff000000ff0000) >> 16;
    let upper_digits = (chunk & 0x000000ff000000ff000000ff000000ff) * 100;
    let chunk = lower_digits + upper_digits;

    // 4-byte mask trick (works on 2 pairs of four digits)
    let lower_digits = (chunk & 0x0000ffff000000000000ffff00000000) >> 32;
    let upper_digits = (chunk & 0x000000000000ffff000000000000ffff) * 10000;
    let chunk = lower_digits + upper_digits;

    // 8-byte mask trick (works on a pair of eight digits)
    let lower_digits = (chunk & 0x00000000ffffffff0000000000000000) >> 64;
    let upper_digits = (chunk & 0x000000000000000000000000ffffffff) * 100000000;
    let chunk = lower_digits + upper_digits;

    let out = chunk as u64;
    debug_assert!(out <= 9999_9999_9999_9999);
    unsafe { std::hint::assert_unchecked(out <= 9999_9999_9999_9999) };

    (out, len)
}

macro_rules! impl_fast_read {
     ($fn:ident; $size:literal; $(($unsigned:ty, $signed:ty)),* ) => {
        $(
             impl FastBinary<$size> for $unsigned {
                 fn take_fast(ptr: *const u8) -> (Self, u8)
                 where
                     Self: Sized,
                 {
                     debug_assert!(
                         !unsafe { *ptr }.is_ascii_whitespace(),
                         "Unexpected Whitespace, did you forget ln()?"
                     );
                     let (out, len) = $fn(ptr);
                     (out as $unsigned, len)
                 }
             }

             impl FastBinary<$size> for $signed {
                 fn take_fast(ptr: *const u8) -> (Self, u8)
                 where
                     Self: Sized,
                 {
                     debug_assert!(
                         !unsafe { *ptr }.is_ascii_whitespace(),
                         "Unexpected Whitespace, did you forget ln()?"
                     );
                     let neg = unsafe { *ptr == b'-' };
                     let (x, len) = $fn(unsafe { ptr.add(neg as usize) });
                     let out = if neg { -(x as $signed) } else { x as $signed };
                     (out, len + neg as u8)
                 }
             }
         )*
     };
 }

impl_fast_read!(parse_1e1; 1; (u8,i8),(u16,i16),(u32,i32),(u64,i64),(usize,isize));
impl_fast_read!(parse_1e2; 2; (u8,i8),(u16,i16),(u32,i32),(u64,i64),(usize,isize));
impl_fast_read!(parse_1e3; 3; (u8,i8),(u16,i16),(u32,i32),(u64,i64),(usize,isize));
impl_fast_read!(parse_1e4; 4; (u16,i16),(u32,i32),(u64,i64),(usize,isize));
impl_fast_read!(parse_1e5; 5; (u16,i16),(u32,i32),(u64,i64),(usize,isize));

// parse_1e8 may be faster here, but parse_1e6 is in general better
impl_fast_read!(parse_1e6; 6; (u32,i32),(u64,i64),(usize,isize));
impl_fast_read!(parse_1e8; 7; (u32,i32),(u64,i64),(usize,isize));
impl_fast_read!(parse_1e8; 8; (u32,i32),(u64,i64),(usize,isize));
impl_fast_read!(parse_1e9; 9; (u32,i32),(u64,i64),(usize,isize));

// 10^9 is very common, but I am unsure if parse_1e16 is fast here
impl_fast_read!(parse_1e16; 10; (u32,i32),(u64,i64),(usize,isize));
impl_fast_read!(parse_1e16; 11; (u64,i64),(usize,isize));
impl_fast_read!(parse_1e16; 12; (u64,i64),(usize,isize));
impl_fast_read!(parse_1e16; 13; (u64,i64),(usize,isize));
impl_fast_read!(parse_1e16; 14; (u64,i64),(usize,isize));
impl_fast_read!(parse_1e16; 15; (u64,i64),(usize,isize));
impl_fast_read!(parse_1e16; 16; (u64,i64),(usize,isize));
impl_fast_read!(parse_1e19; 19; (u64,i64),(usize,isize));

#[inline]
pub fn parse_1e9(data: *const u8) -> (u32, u8) {
    let (mut out, mut len) = parse_1e8(data);
    if len != 8 {
        return (out, len);
    }

    if let Some(x) = unsafe { data.add(8).read() }.checked_sub(b'0') {
        (out * 10 + x as u32, 9)
    } else {
        (out, len)
    }
}

#[inline]
pub fn parse_1e19(data: *const u8) -> (u64, u8) {
    let (mut out, mut len) = parse_1e16(data);
    if len != 16 {
        return (out, len);
    }

    for i in 0..3 {
        unsafe {
            let a = data.add(16 + i).read();
            let Some(n) = a.checked_sub(b'0') else {
                return (out, 16 + i as u8);
            };
            out = out * 10 + n as u64;
        }
    }

    (out, 19)
}

macro_rules! impl_fast_float {
    ($ty:ty; $table_div:ident, $table_mul:ident) => {
        /*
        use
        impl Writable for $ty {
            fn write(self, into: &mut Vec<u8>) {
                use std::fmt::Write;
                let string_slice = unsafe { transmute::<_, &mut String>(into) };
                _ = write!(string_slice, "{self}"); // slow but accurate float
            }
        }*/

        // Blanket impl
        impl<const N: usize> FastBinary<N> for $ty
        where
            $ty: FastFloat<N, N, true>,
        {
            fn take_fast(ptr: *const u8) -> (Self, u8)
            where
                Self: Sized,
            {
                <$ty>::take_float(ptr)
            }
        }

        impl<const N: usize, const M: usize, const NEGATIVE: bool> FastFloat<N, M, NEGATIVE>
            for $ty
        {
            fn take_float(ptr: *const u8) -> (Self, u8)
            where
                Self: Sized,
            {
                unsafe {
                    let mut cursor = ptr;
                    let is_negative = NEGATIVE && *cursor == b'-';
                    debug_assert!(
                        NEGATIVE || (*cursor != b'-'),
                        "Marked as positive float, but got negative"
                    );
                    cursor = cursor.add(is_negative as usize);
                    debug_assert!(
                        (*cursor).is_ascii_digit(),
                        "We must have float that starts prefix with 0-9. Did you forget ln()?"
                    );

                    let (prefix, prefix_len) = match N {
                        1 => ((*cursor - b'0') as $ty, 1),
                        2 => {
                            let (x, len) = parse_1e2(cursor);
                            (x as $ty, len)
                        }
                        3 => {
                            let (x, len) = parse_1e3(cursor);
                            (x as $ty, len)
                        }
                        4 => {
                            let (x, len) = parse_1e4(cursor);
                            (x as $ty, len)
                        }
                        5..=8 => {
                            let (x, len) = parse_1e8(cursor);
                            (x as $ty, len)
                        }
                        9..=16 => {
                            let (x, len) = parse_1e16(cursor);
                            (x as $ty, len)
                        }
                        _ => panic!("Invalid N={N} in take_float"),
                    };
                    cursor = cursor.add(prefix_len as usize);
                    if *cursor != b'.' {
                        return (
                            if is_negative { -prefix } else { prefix },
                            prefix_len + is_negative as u8,
                        );
                    }
                    cursor = cursor.add(1);
                    debug_assert!(
                        (*cursor).is_ascii_digit(),
                        "We must have float that starts suffix with 0-9. Did you forget ln()?"
                    );

                    let (suffix, suffix_len) = match M {
                        1 => ((*cursor - b'0') as $ty * 0.1, 1),
                        2 => {
                            let (x, len) = parse_1e2(cursor);
                            (x as $ty * $table_div[len as usize], len)
                        }
                        3 => {
                            let (x, len) = parse_1e3(cursor);
                            (x as $ty * $table_div[len as usize], len)
                        }
                        4 => {
                            let (x, len) = parse_1e4(cursor);
                            (x as $ty * $table_div[len as usize], len)
                        }
                        5..=8 => {
                            let (x, len) = parse_1e8(cursor);
                            (x as $ty * $table_div[len as usize], len)
                        }
                        9..=16 => {
                            let (x, len) = parse_1e16(cursor);
                            (x as $ty * $table_div[len as usize], len)
                        }
                        _ => {
                            let (x, len) = parse_1e16(cursor);
                            if len == 16 && cursor.add(16).read().is_ascii_digit() {
                                let (_, len2) = parse_1e16(cursor.add(16));
                                (x as $ty * $table_div[len as usize], len2+len)
                            } else {
                                (x as $ty * $table_div[len as usize], len)
                            }
                        }
                    };
                    let sum = prefix + suffix;

                    (
                        if is_negative { -sum } else { sum },
                        suffix_len + prefix_len + 1 + is_negative as u8,
                    )
                }
            }
        }
    };
}

impl_fast_float!(f32; TABLE_F32_DIV, TABLE_F32_MUL);
impl_fast_float!(f64; TABLE_F64_DIV, TABLE_F64_MUL);

macro_rules! impl_fast_intfloat {
    ($ty:ty; $table:ident) => {
        impl<const N: usize, const M: usize, const NEGATIVE: bool> FastFloat<N, M, NEGATIVE>
            for $ty
        {
            fn take_float(ptr: *const u8) -> (Self, u8)
            where
                Self: Sized,
            {
                unsafe {
                    let mut cursor = ptr;
                    let is_negative = NEGATIVE && *cursor == b'-';
                    debug_assert!(
                        NEGATIVE || (*cursor != b'-'),
                        "Marked as positive float, but got negative"
                    );
                    cursor = cursor.add(is_negative as usize);
                    debug_assert!(
                        (*cursor).is_ascii_digit(),
                        "We must have float that starts prefix with 0-9. Did you forget ln()?"
                    );

                    let prefix_mul = $table[M];

                    let (prefix, prefix_len) = match N {
                        1 => ((*cursor - b'0') as $ty, 1),
                        2 => {
                            let (x, len) = parse_1e2(cursor);
                            (x as $ty, len)
                        }
                        3 => {
                            let (x, len) = parse_1e3(cursor);
                            (x as $ty, len)
                        }
                        4 => {
                            let (x, len) = parse_1e4(cursor);
                            (x as $ty, len)
                        }
                        5..=8 => {
                            let (x, len) = parse_1e8(cursor);
                            (x as $ty, len)
                        }
                        9..=16 => {
                            let (x, len) = parse_1e16(cursor);
                            (x as $ty, len)
                        }
                        _ => panic!("Invalid N={N} in take_float"),
                    };
                    cursor = cursor.add(prefix_len as usize);
                    if *cursor != b'.' {
                        return (
                            if is_negative {
                                -prefix * prefix_mul
                            } else {
                                prefix * prefix_mul
                            },
                            prefix_len + is_negative as u8,
                        );
                    }
                    cursor = cursor.add(1);
                    debug_assert!(
                        (*cursor).is_ascii_digit(),
                        "We must have float that starts suffix with 0-9. Did you forget ln()?"
                    );

                    let (suffix, suffix_len) = match M {
                        1 => ((*cursor - b'0') as $ty, 1),
                        2 => {
                            let (x, len) = parse_1e2(cursor);
                            (x as $ty, len)
                        }
                        3 => {
                            let (x, len) = parse_1e3(cursor);
                            (x as $ty, len)
                        }
                        4 => {
                            let (x, len) = parse_1e4(cursor);
                            (x as $ty, len)
                        }
                        5..=8 => {
                            let (x, len) = parse_1e8(cursor);
                            (x as $ty, len)
                        }
                        9..=16 => {
                            let (x, len) = parse_1e16(cursor);
                            (x as $ty, len)
                        }
                        _ => panic!("Invalid M={M} in take_float"),
                    };
                    let suffix_mul = $table[M - suffix_len as usize];

                    let sum = prefix * prefix_mul + suffix * suffix_mul;
                    (
                        if is_negative { -sum } else { sum },
                        suffix_len + prefix_len + 1 + is_negative as u8,
                    )
                }
            }
        }
    };
}

const TABLE_FI32: [i32; 10] = [
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
];

const TABLE_FI64: [i64; 18] = [
    1,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000,
    10000000000,
    100000000000,
    1000000000000,
    10000000000000,
    100000000000000,
    1000000000000000,
    10000000000000000,
    100000000000000000,
];

impl_fast_intfloat!(i32; TABLE_FI32);
impl_fast_intfloat!(i64; TABLE_FI64);

impl<'a> std::ops::Drop for Io<'a> {
    fn drop(&mut self) {
        self.flush();
    }
}

impl<'a> std::fmt::Write for Io<'a> {
    fn write_str(&mut self, s: &str) -> std::fmt::Result {
        self.output.extend_from_slice(s.as_bytes());
        Ok(())
    }
}

impl<'a> Binary<'a> for &'a str {
    fn take(data: &'a [u8]) -> (Self, Move)
    where
        Self: Sized,
    {
        let len = data.iter().take_while(|x| !x.is_ascii_whitespace()).count();
        (
            unsafe { std::str::from_utf8_unchecked(&data[..len]) },
            len as Move,
        )
    }
}

impl<'a> Binary<'a> for &'a [u8] {
    fn take(data: &'a [u8]) -> (Self, Move)
    where
        Self: Sized,
    {
        let len = data.iter().take_while(|x| !x.is_ascii_whitespace()).count();
        (&data[..len], len as Move)
    }
}

impl<'a> Binary<'a> for &'a mut [u8] {
    fn take(data: &'a [u8]) -> (Self, Move)
    where
        Self: Sized,
    {
        let len = data.iter().take_while(|x| !x.is_ascii_whitespace()).count();
        (
            unsafe { std::slice::from_raw_parts_mut(data.as_ptr().cast_mut(), len) },
            len as Move,
        )
    }
}

macro_rules! unsigned {
    ($data:expr) => {{
        debug_assert!(!$data.is_empty(), "Unexpected EOF");
        debug_assert!(
            !$data[0].is_ascii_whitespace(),
            "Unexpected Whitespace, did you forget ln()?"
        );
        let mut out = 0;
        let mut pos = $data.len();
        for (i, &p) in $data.iter().enumerate() {
            let Some(n) = p.checked_sub(b'0') else {
                pos = i;
                break;
            };
            debug_assert!(n <= 9, "unexpected char \'{}\' in parse integer", p as char);

            out = out * 10 + n as Self;
        }
        (out, pos as Move)
    }};
}

macro_rules! impl_num {
      ( $( $unsigned:ty ),*;$( $signed:ty ),*;$( $any:ty ),* ) => {
          $(
              impl Binary<'_> for $unsigned {
                  fn take(data : &[u8]) -> (Self,Move) where Self: Sized {
                      unsigned!(data)
                  }
              }
          )*
          $(
              impl Binary<'_> for $signed {
                  fn take(data : &[u8]) -> (Self,Move) where Self: Sized {
                      if data[0] == b'-' {
                          let (out,pos) = Self::take(&data[1..]);
                          return (-out,pos+1);
                      }
                      unsigned!(data)
                  }
              }
          )*
          $(
              impl<'a> Binary<'a> for $any {
                  fn take(data : &'a [u8]) -> (Self,Move) where Self: Sized {
                      let (text,len) = <&'a str>::take(data);
                      (text.parse().unwrap(), len)
                  }
              }
          )*
      };
  }

impl_num!(u8, u16, u32, u64, u128, usize; i8, i16, i32, i64, i128, isize; f32, f64);

pub const fn setbits64(x: std::num::NonZeroU8) -> u64 {
    u64::MAX >> (64 - x.get())
}

pub const fn setbits128(x: std::num::NonZeroU8) -> u128 {
    u128::MAX >> (128 - x.get())
}

pub const fn parse_alpha_hash_8(data: *const u8) -> (u64, u8) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    let len = ((chunk ^ 0x4040404040404040) & 0x4040404040404040).trailing_zeros() as u8 >> 3;
    (
        chunk & setbits64(unsafe { NonZeroU8::new_unchecked(len << 3) }),
        len,
    )
}

pub const fn parse_alpha_hash_16(data: *const u8) -> (u128, u8) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u128) };
    let len = ((chunk ^ 0x40404040404040404040404040404040) & 0x40404040404040404040404040404040)
        .trailing_zeros() as u8
        >> 3;
    (
        chunk & setbits128(unsafe { NonZeroU8::new_unchecked(len << 3) }),
        len,
    )
}

pub const fn len_alpha_hash_8(data: *const u8) -> u8 {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    ((chunk ^ 0x4040404040404040) & 0x4040404040404040).trailing_zeros() as u8 >> 3
}

pub const fn len_alpha_hash_16(data: *const u8) -> u8 {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u128) };
    ((chunk ^ 0x40404040404040404040404040404040) & 0x40404040404040404040404040404040)
        .trailing_zeros() as u8
        >> 3
}

pub const fn parse_newline_hash_8(data: *const u8) -> (u64, u8) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    let len = (((chunk & 0x2020202020202020) | ((chunk & 0x4040404040404040) >> 1))
        ^ 0x2020202020202020)
        .trailing_zeros() as u8
        >> 3;
    (
        chunk & setbits64(unsafe { NonZeroU8::new_unchecked(len << 3) }),
        len,
    )
}

pub const fn parse_newline_hash_16(data: *const u8) -> (u128, u8) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u128) };
    let len = (((chunk & 0x20202020202020202020202020202020)
        | ((chunk & 0x40404040404040404040404040404040) >> 1))
        ^ 0x20202020202020202020202020202020)
        .trailing_zeros() as u8
        >> 3;
    (
        chunk & setbits128(unsafe { NonZeroU8::new_unchecked(len << 3) }),
        len,
    )
}

/// https://upload.wikimedia.org/wikipedia/commons/d/dd/ASCII-Table.svg
pub const fn parse_alphanumerical_hash_8(data: *const u8) -> (u64, u8) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    let len = (((chunk & 0x4040404040404040) | ((chunk & 0x1010101010101010) << 2))
        ^ 0x4040404040404040)
        .trailing_zeros() as u8
        >> 3;
    (
        chunk & setbits64(unsafe { NonZeroU8::new_unchecked(len << 3) }),
        len,
    )
}

pub const fn parse_alphanumerical_hash_16(data: *const u8) -> (u128, u8) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u128) };
    let len = (((chunk & 0x40404040404040404040404040404040)
        | ((chunk & 0x10101010101010101010101010101010) << 2))
        ^ 0x40404040404040404040404040404040)
        .trailing_zeros() as u8
        >> 3;
    (
        chunk & setbits128(unsafe { NonZeroU8::new_unchecked(len << 3) }),
        len,
    )
}

pub const fn len_alphanumerical_hash_8(data: *const u8) -> u8 {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    (((chunk & 0x4040404040404040) | ((chunk & 0x1010101010101010) << 2)) ^ 0x4040404040404040)
        .trailing_zeros() as u8
        >> 3
}

pub const fn len_alphanumerical_hash_16(data: *const u8) -> u8 {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u128) };
    (((chunk & 0x40404040404040404040404040404040)
        | ((chunk & 0x10101010101010101010101010101010) << 2))
        ^ 0x40404040404040404040404040404040)
        .trailing_zeros() as u8
        >> 3
}

fn len_alphanumerical<const N: usize>(ptr: *const u8) -> usize {
    match N {
        0 => panic!("Cant parse when N = 0"),
        1..=8 => len_alphanumerical_hash_8(ptr) as usize,
        9..=16 => len_alphanumerical_hash_16(ptr) as usize,
        // generic over any N, however no optimization on it
        17.. => {
            let mut moving_ptr = ptr;
            let mut len = len_alphanumerical_hash_8(moving_ptr);
            while len == 8 {
                moving_ptr = unsafe { moving_ptr.add(8) };
                if unsafe { *moving_ptr }.is_ascii_whitespace() {
                    len = 0;
                    break;
                }
                len = len_alphanumerical_hash_8(moving_ptr);
            }
            len as usize + (moving_ptr as usize - ptr as usize)
        }
    }
}

/// Alphanumerical string slice
impl<const N: usize> FastBinary<N> for &[u8] {
    fn take_fast_impl<const ADD: usize>(io: &mut Io<'_>) -> Self
    where
        Self: Sized,
    {
        let ptr = unsafe { io.input_ptr() };
        let len = match N {
            1..=8 => len_alphanumerical_hash_8(ptr) as usize,
            9..=16 => len_alphanumerical_hash_16(ptr) as usize,
            // generic over any N, however no optimization on it
            0 | 17.. => {
                let mut moving_ptr = ptr;
                let mut len = len_alphanumerical_hash_8(moving_ptr);
                while len == 8 {
                    moving_ptr = unsafe { moving_ptr.add(8) };
                    if unsafe { *moving_ptr }.is_ascii_whitespace() {
                        len = 0;
                        break;
                    }
                    len = len_alphanumerical_hash_8(moving_ptr);
                }
                len as usize + (moving_ptr as usize - ptr as usize)
            }
        };
        let out = unsafe { std::slice::from_raw_parts(ptr, len) };
        io.pos += len + ADD;
        out
    }

    fn take_fast(ptr: *const u8) -> (Self, u8)
    where
        Self: Sized,
    {
        todo!()
    }
}

impl<const N: usize> FastBinary<N> for &str {
    fn take_fast(ptr: *const u8) -> (Self, u8)
    where
        Self: Sized,
    {
        let (slice, len) = FastBinary::<N>::take_fast(ptr);
        (unsafe { std::str::from_utf8_unchecked(slice) }, len)
    }
}

macro_rules! impl_hash {
    ($ty:ident, $fn8:ident, $fn16:ident, $whitespace:ident) => {
        impl $ty {
            pub fn new<const N: usize>(data: &str) -> Self {
                FastBinary::<N>::take_fast(data.as_ptr()).0
            }
        }

        impl<const N: usize> FastBinary<N> for ($ty,&[u8]) {
            fn take_fast(ptr: *const u8) -> (Self, u8)
            where
                Self: Sized,
            {
                let (hash,len) = FastBinary::<N>::take_fast(ptr);
                unsafe { ((hash,std::slice::from_raw_parts(ptr, len as usize)),len) }
            }
        }

        impl<const N: usize> FastBinary<N> for ($ty, InputSlice) {
            fn take_fast(ptr: *const u8) -> (Self, u8)
            where
                Self: Sized,
            {
                unreachable!()
            }
            fn take_fast_impl<const ADD: usize>(io: &mut Io<'_>) -> Self
            where
                Self: Sized,
            {
                let offset = io.pos as u32;
                let (hash, len) = FastBinary::<N>::take_fast(unsafe { io.input_ptr() });
                io.pos += len as usize + ADD;
                (
                    hash,
                    InputSlice::new(offset,len as u32),
                )
            }
        }

        impl<'a> Binary<'a> for $ty {
            fn take(data: &'a [u8]) -> (Self, Move)
            where
                Self: Sized {
                todo!()
            }
            fn take_impl<const ADD: usize>(io: &mut Io<'a>) -> Self
                where
                    Self: Sized, {
                let ptr = unsafe { io.input_ptr() };
                let mut moving_ptr = ptr;
                let (mut out, mut len) = $fn8(moving_ptr);
                while len == 8 {
                    moving_ptr = unsafe { moving_ptr.add(8) };
                    if $whitespace(unsafe { *moving_ptr }) {
                        len = 0;
                        break;
                    }
                    let (new_out, new_len) = $fn8(moving_ptr);

                    out = new_out
                        .rotate_left(5)
                        .wrapping_mul(0x517cc1b727220a95)
                        .bitxor(out);
                    len = new_len;
                }
                io.move_input(len as usize + (moving_ptr as usize - ptr as usize) + ADD);
                $ty(out)
            }
        }

        impl<const N: usize> FastBinary<N> for $ty {
            fn take_fast(ptr: *const u8) -> (Self, u8)
            where
                Self: Sized,
            {
                // This hashing is taken from fxhash, but is not exactly the same
                let (hash, len) = match N {
                    0 => panic!("Cant hash when N = 0"),
                    1..=8 => $fn8(ptr),
                    9..=16 => {
                        let (x, len) = $fn16(ptr);
                        let [l, h] = [x as u64, (x >> 64) as u64]; // compiles to transmute, but this way we do not care about endianness

                        (
                            h.rotate_left(5).wrapping_mul(0x517cc1b727220a95).bitxor(l),
                            len,
                        )
                    }
                    // generic over any N, however no optimization on it
                    17..=255 => {
                        let mut moving_ptr = ptr;
                        let (mut out, mut len) = $fn8(moving_ptr);
                        while len == 8 {
                            moving_ptr = unsafe { moving_ptr.add(8) };
                            if $whitespace(unsafe { *moving_ptr }) {
                                len = 0;
                                break;
                            }
                            let (new_out, new_len) = $fn8(moving_ptr);

                            out = new_out
                                .rotate_left(5)
                                .wrapping_mul(0x517cc1b727220a95)
                                .bitxor(out);
                            len = new_len;
                        }
                        (out, len + (moving_ptr as usize - ptr as usize) as u8)
                    }
                    256.. => panic!("Cant hash when N > 255 as the hashing length is too high for an u8"),
                };
                ($ty(hash), len)
            }
        }
    };
}

#[inline(always)]
fn is_ascii_newline(x: u8) -> bool {
    #[cfg(windows)]
    {
        x == b'\n' || x == b'\r'
    }
    #[cfg(not(windows))]
    {
        x == b'\n'
    }
}

#[inline(always)]
fn is_ascii_whitespace(x: u8) -> bool {
    x.is_ascii_whitespace()
}

impl_hash!(
    AlphaHash,
    parse_alpha_hash_8,
    parse_alpha_hash_16,
    is_ascii_whitespace
);
impl_hash!(
    AlphaNumericalHash,
    parse_alphanumerical_hash_8,
    parse_alphanumerical_hash_16,
    is_ascii_whitespace
);
impl_hash!(
    NewlineHash,
    parse_newline_hash_8,
    parse_newline_hash_16,
    is_ascii_newline
);

pub const fn len_newline_hash_8(data: *const u8) -> u8 {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    (((chunk & 0x2020202020202020) | ((chunk & 0x4040404040404040) >> 1)) ^ 0x2020202020202020)
        .trailing_zeros() as u8
        >> 3
}

pub const fn len_newline_hash_16(data: *const u8) -> u8 {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u128) };
    (((chunk & 0x20202020202020202020202020202020)
        | ((chunk & 0x40404040404040404040404040404040) >> 1))
        ^ 0x20202020202020202020202020202020)
        .trailing_zeros() as u8
        >> 3
}

fn len_to_newline(ptr: *const u8) -> usize {
    let mut moving_ptr = ptr;
    let mut len = len_newline_hash_8(moving_ptr);
    while len == 8 {
        moving_ptr = unsafe { moving_ptr.add(8) };
        if unsafe { *moving_ptr } == b'\n' {
            len = 0;
            break;
        }
        len = len_newline_hash_8(moving_ptr);
    }
    len as usize + (moving_ptr as usize - ptr as usize)
}

// Fast stdin

const STDOUT_FILENO: i32 = 1;
const STDIN_FILENO: i32 = 0;

#[cfg(target_os = "windows")]
pub mod sys {
    use std::io::Read;

    use super::*;
    unsafe extern "C" {
        pub fn _fstat64(fd: i32, stat: *mut _stat64) -> i32;
        pub fn _write(fd: i32, buf: *const u8, count: u32) -> i32;
        //pub fn _read(fd: i32, buf: *mut u8, count: u32) -> i32;
    }

    pub fn mmap_stdin() -> &'static [u8] {
        let mut data = vec![];
        let len = stdin_len();
        data.reserve_exact(len.saturating_sub(data.len()));
        unsafe {
            data.set_len(len);
        }
        stdin_read(&mut data[..]);
        data.leak()
    }

    pub fn stdin_len() -> usize {
        let mut stat = _stat64::default();
        unsafe {
            assert_eq!(
                _fstat64(STDIN_FILENO, (&mut stat) as *mut _),
                0,
                "Expected valid fstat"
            )
        };
        assert_ne!(stat.st_size, 0, "Expected non zero st_size");
        stat.st_size as usize
    }

    pub fn stdin_read(data: &mut [u8]) {
        assert_eq!(
            std::io::stdin().read(data).expect("correct read"),
            data.len(),
            "Expected correct read"
        )
    }

    pub fn write_to_end(data: &[u8]) {
        if data.is_empty() {
            return;
        }
        unsafe {
            assert_eq!(
                self::_write(STDOUT_FILENO, data.as_ptr(), data.len() as u32) as usize,
                data.len(),
                "Expected correct write"
            );
        }
    }

    type __time64_t = i64;
    type _dev_t = u32;
    type _ino_t = u16;
    #[repr(C)]
    #[derive(Default)]
    pub struct _stat64 {
        st_dev: _dev_t,
        st_ino: _ino_t,
        st_mode: u16,
        st_nlink: i16,
        st_uid: i16,
        st_gid: i16,
        st_rdev: _dev_t,
        st_size: i64,
        st_atime: __time64_t,
        st_mtime: __time64_t,
        st_ctime: __time64_t,
    }
}

#[cfg(target_os = "linux")]
pub mod sys {
    use super::*;
    extern "C" {
        pub fn read(fd: i32, buf: *mut u8, count: usize) -> isize;
        pub fn fstat(fd: i32, stat: *mut stat) -> i32;
        pub fn write(fd: i32, buf: *const u8, count: usize) -> isize;
        pub fn mmap(
            addr: *mut u8,
            length: usize,
            prot: i32,
            flags: i32,
            fd: i32,
            offset: off_t,
        ) -> *const u8;
    }

    pub fn write_to_end(data: &[u8]) {
        if data.is_empty() {
            return;
        }
        unsafe {
            self::write(STDOUT_FILENO, data.as_ptr(), data.len());
        }
    }

    #[inline]
    pub fn stdin_read(data: &mut [u8]) {
        assert_eq!(
            unsafe { read(STDIN_FILENO, data.as_mut_ptr(), data.len()) } as usize,
            data.len(),
            "Expected correct read"
        );
    }

    #[inline]
    pub fn stdin_len() -> usize {
        let mut stat = stat::default();
        assert!(
            unsafe { fstat(STDIN_FILENO, (&mut stat) as *mut stat) } >= 0,
            "Expected valid fstat"
        );
        assert_ne!(stat.st_size, 0, "Expected non zero st_size");
        stat.st_size as usize
    }

    pub fn mmap_stdin() -> &'static [u8] {
        let len = stdin_len();
        unsafe {
            let ptr = mmap(
                std::ptr::null_mut(),
                len,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE,
                STDIN_FILENO,
                0,
            );
            std::slice::from_raw_parts(ptr, len)
        }
    }

    const PROT_READ: i32 = 1;
    const PROT_WRITE: i32 = 2;
    const MAP_PRIVATE: i32 = 0x0002;
    pub type dev_t = u64;
    pub type ino_t = u64;
    pub type nlink_t = u64;
    pub type mode_t = u32;
    pub type uid_t = u32;
    pub type gid_t = u32;
    pub type c_int = i32;
    pub type off_t = i64;
    pub type blkcnt_t = i64;
    pub type time_t = i64;
    pub type blksize_t = i64;
    #[repr(C)]
    #[derive(Default)]
    pub struct stat {
        pub st_dev: dev_t,
        pub st_ino: ino_t,
        pub st_nlink: nlink_t,
        pub st_mode: mode_t,
        pub st_uid: uid_t,
        pub st_gid: gid_t,
        __pad0: c_int,
        pub st_rdev: dev_t,
        pub st_size: off_t,
        pub st_blksize: blksize_t,
        pub st_blocks: blkcnt_t,
        pub st_atime: time_t,
        pub st_atime_nsec: i64,
        pub st_mtime: time_t,
        pub st_mtime_nsec: i64,
        pub st_ctime: time_t,
        pub st_ctime_nsec: i64,
        __unused: [i64; 3],
    }
}
