// https://pokval26.kattis.com/contests/pokval26/problems/lojtnantslotto
#![allow(unused, non_camel_case_types)]
const MUL: [u32; 5] = [60, 30, 20, 15, 12];

/* ==================================================================================================== fast_io_ln.rs ==================================================================================================== */
/*
 * Author: Vincent Lagerros vinlag@kth.se
 * Date: 2024-02-21
 *
 * Fast and self-contained unsafe IO for kattis.
 * This is optmized for speed, and can be *very* unsafe if you do not do the input correctly.
 *
 * You have to declare when newlines are used to avoid crashing on windows due to \r\n.
 */

use std::{
    mem::transmute,
    num::{NonZeroU8, NonZeroU32},
    ops::BitXor,
};

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
pub type Move = u32;

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
    fn take_fast(ptr: *const u8) -> (Self, u32)
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
    fn take_float(ptr: *const u8) -> (Self, u32)
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
    fn take_fast(ptr: *const u8) -> (Self, u32)
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
pub trait PadWritable<const N: usize, const C: u8> {
    fn pad_write(self, into: &mut Vec<u8>);
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
        if self.output.len() > 0x4000 - 30 {
            self.flush();
        }
    }

    pub fn output_hint(&mut self, len: usize) {
        self.output
            .reserve_exact(len.saturating_sub(self.output.len()));
    }

    /// Malformed but slightly faster output without write!
    #[inline]
    pub fn write_pad<const N: usize, const C: u8>(&mut self, content: impl PadWritable<N, C>) {
        content.pad_write(&mut self.output);
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
        /*let len = sys::stdin_len();
        let mut text = Vec::with_capacity(len);
        unsafe {
            text.set_len(len);
        }
        sys::stdin_read(&mut text);
        let out = Self {
            pos: 0,
            input: Box::new(text).leak::<'static>(),
            output: Vec::new(),
        };
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
            let len = len_to_newline_both(unsafe { self.input_ptr() });
            let end = self.pos + len;
            let out = unsafe { std::str::from_utf8_unchecked(&self.input[self.pos..end]) };
            self.pos += len + 1;
            out
        }
    }

    /// Consumes an entire line, not to be confused with `nextln`
    pub fn next_line_alpha(&mut self) -> &'a str {
        #[cfg(target_os = "windows")]
        {
            self.next_line()
        }
        #[cfg(not(target_os = "windows"))]
        {
            let len = len_to_newline_alpha(unsafe { self.input_ptr() });
            let end = self.pos + len;
            let out = unsafe { std::str::from_utf8_unchecked(&self.input[self.pos..end]) };
            self.pos += len + 1;
            out
        }
    }

    /// Consumes an entire line, not to be confused with `nextln`
    pub fn next_line_numerical(&mut self) -> &'a str {
        #[cfg(target_os = "windows")]
        {
            self.next_line()
        }
        #[cfg(not(target_os = "windows"))]
        {
            let len = len_to_newline_dig(unsafe { self.input_ptr() });
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

    /// Moves the cursor forwards the specified amount of lines, skipping them
    pub fn move_lines(&mut self, lines: usize) {
        self.move_input(skip_lines_simd::<b'\n'>(&self.input[self.pos..], lines));
    }

    pub fn move_space<const N: usize>(&mut self) {
        //const { assert!(N <= 32, "move_space not implemented for N > 32") }
        self.move_input(skip_32::<b' '>(unsafe { self.input_ptr() }));
    }

    pub fn move_line<const N: usize>(&mut self) {
        //const { assert!(N <= 32, "move_line not implemented for N > 32") }
        self.move_input(skip_32::<b'\n'>(unsafe { self.input_ptr() }));
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

// Write 2 digits at the same time
const WRITE_TABLE: [u16; 100] = [
    12336, 12592, 12848, 13104, 13360, 13616, 13872, 14128, 14384, 14640, 12337, 12593, 12849,
    13105, 13361, 13617, 13873, 14129, 14385, 14641, 12338, 12594, 12850, 13106, 13362, 13618,
    13874, 14130, 14386, 14642, 12339, 12595, 12851, 13107, 13363, 13619, 13875, 14131, 14387,
    14643, 12340, 12596, 12852, 13108, 13364, 13620, 13876, 14132, 14388, 14644, 12341, 12597,
    12853, 13109, 13365, 13621, 13877, 14133, 14389, 14645, 12342, 12598, 12854, 13110, 13366,
    13622, 13878, 14134, 14390, 14646, 12343, 12599, 12855, 13111, 13367, 13623, 13879, 14135,
    14391, 14647, 12344, 12600, 12856, 13112, 13368, 13624, 13880, 14136, 14392, 14648, 12345,
    12601, 12857, 13113, 13369, 13625, 13881, 14137, 14393, 14649,
];

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
            fn write(self, into: &mut Vec<u8>) {
                let mut x = self;
                let add = $fn(x) as usize;
                unsafe {
                    into.reserve(add);
                    into.set_len(into.len() + add);
                }
                let mut i = unsafe { into.as_mut_ptr().add(into.len()) };
                i = unsafe { i.sub(2) };
                for _ in 0..add >> 1 {
                    let index = x % 100;
                    x /= 100;
                    unsafe {
                        (i as *mut u16).write_unaligned(WRITE_TABLE[index as usize]);
                    };
                    i = unsafe { i.sub(2) };
                }
                if add & 1 != 0 {
                    unsafe { i.add(1).write(x as u8 + b'0') };
                }
            }
        }
        impl Writable for $signed {
            fn write(self, into: &mut Vec<u8>) {
                let mut x = self.unsigned_abs();
                let mut len = into.len();
                let add = $fn(x) as usize;
                let reserve = add + (self < 0) as usize;
                unsafe {
                    into.reserve(reserve);
                    into.as_mut_ptr().add(len).write(b'-');
                    len += reserve;
                    into.set_len(len);
                }
                let mut i = unsafe { into.as_mut_ptr().add(len) };
                i = unsafe { i.sub(2) };
                for _ in 0..add >> 1 {
                    let index = x % 100;
                    x /= 100;
                    unsafe {
                        (i as *mut u16).write_unaligned(WRITE_TABLE[index as usize]);
                    };
                    i = unsafe { i.sub(2) };
                }
                if add & 1 != 0 {
                    unsafe { i.add(1).write(x as u8 + b'0') };
                }
            }
        }

        impl<const N: usize, const C: u8> PadWritable<N, C> for $unsigned {
            fn pad_write(self, into: &mut Vec<u8>) {
                let mut len = into.len() + N;
                unsafe {
                    into.resize(into.len() + N, C);
                    into.as_mut_ptr().add(into.len()).write(b'-');
                    into.set_len(len);
                }
                let mut i = unsafe { into.as_mut_ptr().add(len) };
                i = unsafe { i.sub(2) };
                let mut x = self;
                while x > 99 {
                    let index = x % 100;
                    x /= 100;
                    unsafe {
                        (i as *mut u16).write_unaligned(WRITE_TABLE[index as usize]);
                    };
                    i = unsafe { i.sub(2) };
                }
                if x >= 10 {
                    unsafe {
                        (i as *mut u16).write_unaligned(WRITE_TABLE[x as usize]);
                    };
                } else {
                    unsafe { i.add(1).write(x as u8 + b'0') };
                }
            }
        }

        impl<const N: usize, const C: u8> PadWritable<N, C> for $signed {
            fn pad_write(self, into: &mut Vec<u8>) {
                let mut x = self.unsigned_abs();
                let mut len = into.len();
                unsafe {
                    into.resize(into.len() + N, C);
                    len += N;
                    into.set_len(len);
                }
                let mut i = unsafe { into.as_mut_ptr().add(len) };
                i = unsafe { i.sub(2) };
                while x > 99 {
                    let index = x % 100;
                    x /= 100;
                    unsafe {
                        (i as *mut u16).write_unaligned(WRITE_TABLE[index as usize]);
                    };
                    i = unsafe { i.sub(2) };
                }
                if x >= 10 {
                    unsafe {
                        (i as *mut u16).write_unaligned(WRITE_TABLE[x as usize]);
                        if self < 0 {
                            i.sub(1).write(b'-');
                        }
                    };
                } else {
                    unsafe {
                        i.add(1).write(x as u8 + b'0');
                        if self < 0 {
                            i.write(b'-');
                        }
                    };
                }
            }
        }
    };
}

const TABLE_16: [u64; 6] = [0, 9, 99, 999, 9999, u64::MAX];
const TABLE_32: [u64; 11] = [
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
    u64::MAX,
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

macro_rules! impl_read {
     ($size:literal; $(($unsigned:ty, $signed:ty)),* ) => {
        $(
             impl FastBinary<$size> for $unsigned {
                 fn take_fast(ptr: *const u8) -> (Self, u32)
                 where
                     Self: Sized,
                 {
                     debug_assert!(
                         !unsafe { *ptr }.is_ascii_whitespace(),
                         "Unexpected Whitespace, did you forget ln()?"
                     );
                     let (out, len) = parse_integer::<$size>(ptr);
                     (out as $unsigned, len)
                 }
             }

             impl FastBinary<$size> for $signed {
                 fn take_fast(ptr: *const u8) -> (Self, u32)
                 where
                     Self: Sized,
                 {
                     debug_assert!(
                         !unsafe { *ptr }.is_ascii_whitespace(),
                         "Unexpected Whitespace, did you forget ln()?"
                     );
                     let neg = unsafe { *ptr == b'-' };
                     let (x, len) = parse_integer::<$size>(unsafe { ptr.add(neg as usize) });
                     let out = if neg { -(x as $signed) } else { x as $signed };
                     (out, len + neg as u32)
                 }
             }
         )*
     };
 }

impl_read!(1; (u8,i8),(u16,i16),(u32,i32),(u64,i64),(usize,isize));
impl_read!(2; (u8,i8),(u16,i16),(u32,i32),(u64,i64),(usize,isize));
impl_read!(3; (u8,i8),(u16,i16),(u32,i32),(u64,i64),(usize,isize));
impl_read!(4; (u16,i16),(u32,i32),(u64,i64),(usize,isize));
impl_read!(5; (u16,i16),(u32,i32),(u64,i64),(usize,isize));

impl_read!(6; (u32,i32),(u64,i64),(usize,isize));
impl_read!(7; (u32,i32),(u64,i64),(usize,isize));
impl_read!(8; (u32,i32),(u64,i64),(usize,isize));
impl_read!(9; (u32,i32),(u64,i64),(usize,isize));

impl_read!(10; (u32,i32),(u64,i64),(usize,isize));
impl_read!(11; (u64,i64),(usize,isize));
impl_read!(12; (u64,i64),(usize,isize));
impl_read!(13; (u64,i64),(usize,isize));
impl_read!(14; (u64,i64),(usize,isize));
impl_read!(15; (u64,i64),(usize,isize));
impl_read!(16; (u64,i64),(usize,isize));
impl_read!(19; (u64,i64),(usize,isize));

pub fn parse_integer<const N: usize>(data: *const u8) -> (u64, u32) {
    /*const {
        assert!(
            N == 19 || (N >= 1 && N <= 16),
            "read_integer is not supported with that N"
        )
    }*/
    match N {
        1 | 2 | 3 | 4 | 5 | 6 => parse_1e_n::<N>(data),
        // parse_1e8 may be faster here, but parse_1e_n is better
        // in general better for cases with a lot of small numbers
        7 | 8 => parse_1e8(data),
        9 | 10 | 11 | 12 | 13 | 14 => parse_1e8p::<N>(data),
        15 | 16 => parse_1e16(data),
        17 | 18 | 19 | 20 => parse_1e16p::<N>(data),
        _ => unreachable!(),
    }
}

/// Parses digits of a base 10 number, and return it, and its length
///
/// Compiled down to ~6 instructions / dig, this near optimal considering
///
/// ```x86
///    lea     rax, [rax + 4*rax]       <- mul 10
///    lea     rax, [rcx + 2*rax - 48]  <- mul 10 - b'0'
///    movzx   ecx, byte ptr [rdi + 3]  <- data[i] as u64
///    mov     dl, 3                    <- i as u8
///    cmp     rcx, 48                  <- checked_sub
///    jb      .LBB0_6                  <- return
/// ```
#[inline]
pub fn parse_1e_n<const N: usize>(data: *const u8) -> (u64, u32) {
    // Branching like this is faster than a non branching solution like parse_1e8
    // Small numbers are faster given that 4 digits ~32 instructions like parse_1e8
    let x = unsafe { *data };
    let mut out = (x - b'0') as u64;
    for i in 1..N {
        // It is like 1 instruction faster/loop to just work with u64 everywhere
        // On the other hand using u16 is much slower
        let x = unsafe { *data.add(i) as u64 };
        let Some(n) = x.checked_sub(b'0' as u64) else {
            debug_assert!(out <= TABLE_64[N]);
            // unsafe { std::hint::assert_unchecked(out <= TABLE_64[N]) };
            return (out, i as u32);
        };
        debug_assert!(
            n <= 9,
            "unexpected char \'{}\' in parse integer",
            x as u8 as char
        );

        out = out * 10 + n as u64;
    }

    debug_assert!(out <= TABLE_64[N]);
    // unsafe { std::hint::assert_unchecked(out <= TABLE_64[N]) };
    (out, N as u32)
}

/// Parses up to 8 digits of a base 10 number, and return is and the length
/// ~32 instructions
#[inline]
pub fn parse_1e8(data: *const u8) -> (u64, u32) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) }; // max 8 chars

    let zero_to_nine = chunk ^ 0x3030303030303030;
    let non_zero = zero_to_nine & 0xF0F0F0F0F0F0F0F0;
    let len = non_zero.trailing_zeros() >> 3;

    // https://lemire.me/blog/2022/01/21/swar-explained-parsing-eight-digits/
    let chunk = zero_to_nine << (8 * (8 - len));
    const MASK: u64 = 0x000000FF000000FF;
    const MUL1: u64 = 0x000F424000000064; // 100 + (1000000ULL << 32)
    const MUL2: u64 = 0x0000271000000001; // 1 + (10000ULL << 32)
    let chunk = (chunk.wrapping_mul(10)).wrapping_add(chunk >> 8); // val = (val * 2561) >> 8;
    let out = (((chunk & MASK).wrapping_mul(MUL1))
        .wrapping_add(((chunk >> 16) & MASK).wrapping_mul(MUL2)))
        >> 32;
    debug_assert!(out <= 9999_9999);
    // unsafe { std::hint::assert_unchecked(out <= 9999_9999) };
    (out, len)
}

/// Parses up to 16 digits of a base 10 number, and return is and the length
/// ~64 instructions
#[inline]
pub fn parse_1e16(data: *const u8) -> (u64, u32) {
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
    let len = non_zero.trailing_zeros() >> 3;
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
    let chunk = lower_digits as u64 + upper_digits as u64;

    let out = chunk;
    debug_assert!(out <= 9999_9999_9999_9999);
    // unsafe { std::hint::assert_unchecked(out <= 9999_9999_9999_9999) };

    (out, len)
}

#[inline]
pub fn parse_1e8p<const N: usize>(data: *const u8) -> (u64, u32) {
    const MASK: u64 = 0x000000FF000000FF;
    const MUL1: u64 = 0x000F424000000064; // 100 + (1000000ULL << 32)
    const MUL2: u64 = 0x0000271000000001; // 1 + (10000ULL << 32)

    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) }; // max 8 chars

    let zero_to_nine = chunk ^ 0x3030303030303030;
    let non_zero = zero_to_nine & 0xF0F0F0F0F0F0F0F0;
    if non_zero == 0 {
        let chunk = zero_to_nine;
        let chunk = (chunk.wrapping_mul(10)).wrapping_add(chunk >> 8); // val = (val * 2561) >> 8;
        let out = (((chunk & MASK).wrapping_mul(MUL1))
            .wrapping_add(((chunk >> 16) & MASK).wrapping_mul(MUL2)))
            >> 32;
        debug_assert!(out <= 9999_9999);
        // unsafe { std::hint::assert_unchecked(out <= 9999_9999) };

        let mut out = out;
        for i in 8..N {
            // It is like 1 instruction faster/loop to just work with u64 everywhere
            // On the other hand using u16 is much slower
            let x = unsafe { *data.add(i) as u64 };
            let Some(n) = x.checked_sub(b'0' as u64) else {
                return (out, i as u32);
            };
            debug_assert!(
                n <= 9,
                "unexpected char \'{}\' in parse integer",
                x as u8 as char
            );

            out = out * 10 + n as u64;
        }
        return (out, N as u32);
    }

    let len = non_zero.trailing_zeros() >> 3;

    // https://lemire.me/blog/2022/01/21/swar-explained-parsing-eight-digits/
    let chunk = zero_to_nine << (8 * (8 - len));
    let chunk = (chunk.wrapping_mul(10)).wrapping_add(chunk >> 8); // val = (val * 2561) >> 8;
    let out = (((chunk & MASK).wrapping_mul(MUL1))
        .wrapping_add(((chunk >> 16) & MASK).wrapping_mul(MUL2)))
        >> 32;
    debug_assert!(out <= 9999_9999);
    // unsafe { std::hint::assert_unchecked(out <= 9999_9999) };

    (out, len)
}

#[inline]
pub fn parse_1e16p<const N: usize>(data: *const u8) -> (u64, u32) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u128) }; // max 16 chars

    let zero_to_nine = chunk ^ 0x30303030303030303030303030303030;
    let non_zero = zero_to_nine & 0xF0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0;

    if non_zero == 0 {
        let chunk = zero_to_nine;
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
        let chunk = lower_digits as u64 + upper_digits as u64;

        let out = chunk;
        debug_assert!(out <= 9999_9999);
        // unsafe { std::hint::assert_unchecked(out <= 9999_9999) };

        let mut out = out;
        for i in 16..N {
            // It is like 1 instruction faster/loop to just work with u64 everywhere
            // On the other hand using u16 is much slower
            let x = unsafe { *data.add(i) as u64 };
            let Some(n) = x.checked_sub(b'0' as u64) else {
                return (out, i as u32);
            };
            debug_assert!(
                n <= 9,
                "unexpected char \'{}\' in parse integer",
                x as u8 as char
            );

            out = out * 10 + n as u64;
        }
        return (out, N as u32);
    }

    let len = non_zero.trailing_zeros() >> 3;
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
    let chunk = lower_digits as u64 + upper_digits as u64;

    let out = chunk;
    debug_assert!(out <= 9999_9999_9999_9999);
    // unsafe { std::hint::assert_unchecked(out <= 9999_9999_9999_9999) };

    (out, len)
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
            fn take_fast(ptr: *const u8) -> (Self, u32)
            where
                Self: Sized,
            {
                <$ty>::take_float(ptr)
            }
        }

        impl<const N: usize, const M: usize, const NEGATIVE: bool> FastFloat<N, M, NEGATIVE>
            for $ty
        {
            fn take_float(ptr: *const u8) -> (Self, u32)
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

                    let (prefix, prefix_len) = parse_integer::<N>(cursor);
                    cursor = cursor.add(prefix_len as usize);
                    if *cursor != b'.' || !cursor.add(1).read().is_ascii_digit() {
                        return (
                            if is_negative {
                                (-(prefix as i64)) as $ty
                            } else {
                                prefix as i64 as $ty
                            },
                            prefix_len + is_negative as u32,
                        );
                    }
                    let (suffix, suffix_len) = parse_integer::<M>(cursor.add(1));
                    let sum = prefix as i64 as $ty
                        + suffix as i64 as $ty * $table_div[suffix_len as usize];

                    (
                        if is_negative { -sum } else { sum },
                        suffix_len + prefix_len + 1 + is_negative as u32,
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
            fn take_float(ptr: *const u8) -> (Self, u32)
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

                    let (prefix, prefix_len) = parse_integer::<N>(cursor);
                    let prefix_sum = (prefix as $ty) * prefix_mul;
                    cursor = cursor.add(prefix_len as usize);
                    if *cursor != b'.' || !cursor.add(1).read().is_ascii_digit() {
                        return (
                            if is_negative { -prefix_sum } else { prefix_sum },
                            prefix_len + is_negative as u32,
                        );
                    }
                    let (suffix, suffix_len) = parse_integer::<M>(cursor.add(1));
                    let suffix_mul = $table[M - suffix_len as usize];
                    let sum = prefix_sum + (suffix as $ty) * suffix_mul;
                    (
                        if is_negative { -sum } else { sum },
                        suffix_len + prefix_len + 1 + is_negative as u32,
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

pub const fn setbits64(x: std::num::NonZeroU32) -> u64 {
    u64::MAX >> (64 - x.get())
}

pub const fn setbits128(x: std::num::NonZeroU32) -> u128 {
    u128::MAX >> (128 - x.get())
}

pub const fn parse_alpha_hash_8(data: *const u8) -> (u64, u32) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    let len = ((chunk ^ 0x4040404040404040) & 0x4040404040404040).trailing_zeros() >> 3;
    (
        chunk & setbits64(unsafe { NonZeroU32::new_unchecked(len << 3) }),
        len,
    )
}

pub const fn len_alpha_hash_8(data: *const u8) -> u8 {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    ((chunk ^ 0x4040404040404040) & 0x4040404040404040).trailing_zeros() as u8 >> 3
}

pub const fn parse_newline_hash_8(data: *const u8) -> (u64, u32) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    let len = (((chunk & 0x2020202020202020) | ((chunk & 0x4040404040404040) >> 1))
        ^ 0x2020202020202020)
        .trailing_zeros()
        >> 3;
    (
        chunk & setbits64(unsafe { NonZeroU32::new_unchecked(len << 3) }),
        len,
    )
}

/// https://upload.wikimedia.org/wikipedia/commons/d/dd/ASCII-Table.svg
pub const fn parse_alphanumerical_hash_8(data: *const u8) -> (u64, u32) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    let len = (((chunk & 0x4040404040404040) | ((chunk & 0x1010101010101010) << 2))
        ^ 0x4040404040404040)
        .trailing_zeros()
        >> 3;
    (
        chunk & setbits64(unsafe { NonZeroU32::new_unchecked(len << 3) }),
        len,
    )
}

/// Can be changed to accept - by doing "(chunk & 0x01010101010101010101010101010101) << 6"
pub const fn parse_alphanumerical_hash_16(data: *const u8) -> (u128, u32) {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u128) };
    let len = (((chunk & 0x40404040404040404040404040404040)
        | ((chunk & 0x10101010101010101010101010101010) << 2))
        ^ 0x40404040404040404040404040404040)
        .trailing_zeros()
        >> 3;
    (
        chunk & setbits128(unsafe { NonZeroU32::new_unchecked(len << 3) }),
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

    fn take_fast(ptr: *const u8) -> (Self, u32)
    where
        Self: Sized,
    {
        todo!()
    }
}

impl<const N: usize> FastBinary<N> for &str {
    fn take_fast(ptr: *const u8) -> (Self, u32)
    where
        Self: Sized,
    {
        let (slice, len) = FastBinary::<N>::take_fast(ptr);
        (unsafe { std::str::from_utf8_unchecked(slice) }, len)
    }
}

macro_rules! impl_hash {
    ($ty:ident, $fn8:ident, $whitespace:ident) => {
        impl $ty {
            pub fn new<const N: usize>(data: &str) -> Self {
                FastBinary::<N>::take_fast(data.as_ptr()).0
            }
        }

        impl<const N: usize> FastBinary<N> for ($ty, &[u8]) {
            fn take_fast(ptr: *const u8) -> (Self, u32)
            where
                Self: Sized,
            {
                let (hash, len) = FastBinary::<N>::take_fast(ptr);
                unsafe { ((hash, std::slice::from_raw_parts(ptr, len as usize)), len) }
            }
        }

        impl<const N: usize> FastBinary<N> for ($ty, InputSlice) {
            fn take_fast(ptr: *const u8) -> (Self, u32)
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
                (hash, InputSlice::new(offset, len as u32))
            }
        }

        impl<'a> Binary<'a> for $ty {
            fn take(data: &'a [u8]) -> (Self, Move)
            where
                Self: Sized,
            {
                todo!()
            }
            fn take_impl<const ADD: usize>(io: &mut Io<'a>) -> Self
            where
                Self: Sized,
            {
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
            fn take_fast(ptr: *const u8) -> (Self, u32)
            where
                Self: Sized,
            {
                // This hashing is taken from fxhash, but is not exactly the same
                let (hash, len) = match N {
                    1..=8 => $fn8(ptr),
                    9..=16 => {
                        let mut moving_ptr = ptr;
                        let (mut out, len) = $fn8(moving_ptr);
                        if len == 8 {
                            moving_ptr = unsafe { moving_ptr.add(8) };
                            if $whitespace(unsafe { *moving_ptr }) {
                                (out, 8)
                            } else {
                                let (new_out, new_len) = $fn8(moving_ptr);

                                out = new_out
                                    .rotate_left(5)
                                    .wrapping_mul(0x517cc1b727220a95)
                                    .bitxor(out);
                                (out, len + new_len)
                            }
                        } else {
                            (out, len)
                        }
                    }
                    // generic over any N, however no optimization on it
                    17..=255 | 0 => {
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
                        (out, len + (moving_ptr as usize - ptr as usize) as u32)
                    }
                    256.. => {
                        panic!("Cant hash when N > 255 as the hashing length is too high for an u8")
                    }
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

impl_hash!(AlphaHash, parse_alpha_hash_8, is_ascii_whitespace);
impl_hash!(
    AlphaNumericalHash,
    parse_alphanumerical_hash_8,
    is_ascii_whitespace
);
impl_hash!(NewlineHash, parse_newline_hash_8, is_ascii_newline);

pub const fn len_newline_hash_8(data: *const u8) -> u32 {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u64) };
    (((chunk & 0x2020202020202020) | ((chunk & 0x4040404040404040) >> 1)) ^ 0x2020202020202020)
        .trailing_zeros()
        >> 3
}

pub const fn len_newline_hash_16(data: *const u8) -> u32 {
    let chunk = unsafe { std::ptr::read_unaligned(data as *const u128) };
    (((chunk & 0x20202020202020202020202020202020)
        | ((chunk & 0x40404040404040404040404040404040) >> 1))
        ^ 0x20202020202020202020202020202020)
        .trailing_zeros()
        >> 3
}
fn skip_lines<const C: u8>(io_ptr: *const u8, mut skips: usize) -> usize {
    let mut add = 0;
    while skips != 0 {
        skips -= (unsafe { *io_ptr.add(add) } == C) as usize;
        add += 1;
    }
    add
}

fn skip_32<const C: u8>(ptr: *const u8) -> usize {
    unsafe {
        use std::arch::x86_64::*;
        let expected = _mm256_set1_epi8(C as i8);
        let load = _mm256_loadu_si256(ptr as *const _);
        let cmp = _mm256_cmpeq_epi8(load, expected);
        let mask = _mm256_movemask_epi8(cmp);
        (mask.trailing_zeros() + 1) as usize
    }
}

fn skip_lines_simd<const C: u8>(slice: &[u8], skips: usize) -> usize {
    if skips == 0 {
        return 0;
    }
    if slice.len() < 32 {
        return skip_lines::<C>(slice.as_ptr(), skips as usize);
    }
    let mut skips = skips as u32;
    let mut add = 0;

    unsafe {
        use std::arch::x86_64::*;
        let offset = slice.as_ptr().align_offset(32);

        if slice.len() < offset {
            return skip_lines::<C>(slice.as_ptr(), skips as usize);
        }

        let expected = _mm256_set1_epi8(C as i8);
        {
            let load = _mm256_loadu_si256(slice.as_ptr() as *const _);
            let cmp = _mm256_cmpeq_epi8(load, expected);
            let mask = _mm256_movemask_epi8(cmp) as u32 & ((1 << offset) - 1);
            let ones = mask.count_ones();
            if ones >= skips {
                let mut mask = mask;
                loop {
                    let zeros = mask.trailing_zeros();
                    mask ^= 1 << zeros;
                    skips -= 1;
                    if skips == 0 {
                        return add + 1 + zeros as usize;
                    }
                }
            }
            add += offset;
            skips -= ones;
        }

        let mut it = slice[offset..].chunks_exact(32);
        for ptr in it.by_ref() {
            let load = _mm256_load_si256(ptr.as_ptr() as *const _);
            let cmp = _mm256_cmpeq_epi8(load, expected);
            let mask = _mm256_movemask_epi8(cmp) as u32;
            let ones = mask.count_ones(); // low bits = lower values
            if ones < skips {
                add += 32;
                skips -= ones;
                continue;
            }
            let mut mask = mask;
            loop {
                let zeros = mask.trailing_zeros();
                mask ^= 1 << zeros;
                skips -= 1;
                if skips == 0 {
                    return add + 1 + zeros as usize;
                }
            }
        }
        add += skip_lines::<C>(it.remainder().as_ptr(), skips as usize);
        add
    }
}

fn len_to_newline_both(ptr: *const u8) -> usize {
    let mut moving_ptr = ptr;

    let chunk = unsafe { std::ptr::read_unaligned(moving_ptr as *const u64) };

    // remove 0x4040404040404040 if only dig
    let mut len =
        (((chunk & 0x2020202020202020) | ((chunk & 0x4040404040404040) >> 1)) ^ 0x2020202020202020);

    while len == 0 {
        moving_ptr = unsafe { moving_ptr.add(8) };
        // if unsafe { *moving_ptr } == b'\n' {
        //     return (moving_ptr as usize - ptr as usize);
        // }

        let chunk = unsafe { std::ptr::read_unaligned(moving_ptr as *const u64) };
        len = (((chunk & 0x2020202020202020) | ((chunk & 0x4040404040404040) >> 1))
            ^ 0x2020202020202020);
    }

    (len.trailing_zeros() >> 3) as usize + (moving_ptr as usize - ptr as usize)
}

fn len_to_newline_dig(ptr: *const u8) -> usize {
    let mut moving_ptr = ptr;

    let chunk = unsafe { std::ptr::read_unaligned(moving_ptr as *const u64) };

    let mut len = ((chunk & 0x2020202020202020) ^ 0x2020202020202020);

    while len == 0 {
        moving_ptr = unsafe { moving_ptr.add(8) };
        // if unsafe { *moving_ptr } == b'\n' {
        //     return (moving_ptr as usize - ptr as usize);
        // }

        let chunk = unsafe { std::ptr::read_unaligned(moving_ptr as *const u64) };
        len = ((chunk & 0x2020202020202020) ^ 0x2020202020202020);
    }

    (len.trailing_zeros() >> 3) as usize + (moving_ptr as usize - ptr as usize)
}

fn len_to_newline_alpha(ptr: *const u8) -> usize {
    let mut moving_ptr = ptr;

    let chunk = unsafe { std::ptr::read_unaligned(moving_ptr as *const u64) };

    let mut len = ((chunk & 0x4040404040404040) ^ 0x4040404040404040);

    while len == 0 {
        moving_ptr = unsafe { moving_ptr.add(8) };
        // if unsafe { *moving_ptr } == b'\n' {
        //     return (moving_ptr as usize - ptr as usize);
        // }

        let chunk = unsafe { std::ptr::read_unaligned(moving_ptr as *const u64) };
        len = ((chunk & 0x4040404040404040) ^ 0x4040404040404040);
    }

    (len.trailing_zeros() >> 3) as usize + (moving_ptr as usize - ptr as usize)
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

/* ==================================================================================================== fast_io_ln.rs ==================================================================================================== */

/* ==================================================================================================== radix_sort.rs ==================================================================================================== */

/*
 * Author: Vincent Lagerros vinlag@kth.se
 * Date: 2026-02-12
 *
 * Fast sorting for integers of size 8-32 signed and unsigned
 */

use std::mem::swap;

pub trait Sorting<T> {
    fn sort_fast<P>(&mut self)
    where
        T: SortingPrimative<P>,
    {
        self.sort_fast_reuse(&mut vec![])
    }
    fn sort_fast_reuse<P>(&mut self, tmp: &mut Vec<T>)
    where
        T: SortingPrimative<P>;
}

impl<T> Sorting<T> for Vec<T> {
    fn sort_fast_reuse<P>(&mut self, tmp: &mut Vec<T>)
    where
        T: SortingPrimative<P>,
    {
        T::sort_fast_reuse(self, tmp);
    }
}

pub trait SortingPrimative<P>: Sized {
    fn sort_fast_reuse(output: &mut Vec<Self>, tmp: &mut Vec<Self>);
}
macro_rules! impl_primative {
    ($as:ty, $count:expr; $for:ty, $transmuted_into:ty) => {
        impl SortingPrimative<$as> for $for {
            fn sort_fast_reuse(output: &mut Vec<Self>, tmp: &mut Vec<Self>) {
                let output = unsafe { transmute::<_, &mut Vec<$transmuted_into>>(output) };
                let tmp = unsafe { transmute::<_, &mut Vec<$transmuted_into>>(tmp) };
                radix_sort::<$count, _, _>(output, tmp);
            }
        }
    };
}

impl_primative!(u16, 2; u16, u16);
impl_primative!(u16, 2; i16, u16);

impl_primative!(i16, 2; u16, i16);
impl_primative!(i16, 2; i16, i16);

impl_primative!(u32, 4; u32, u32);
impl_primative!(u32, 4; i32, u32);

impl_primative!(i32, 4; u32, i32);
impl_primative!(i32, 4; i32, i32);

impl_primative!(u24, 3; u32, u32);
impl_primative!(u24, 3; i32, u32);

impl_primative!(i24, 3; u32, i32);
impl_primative!(i24, 3; i32, i32);

impl_primative!(i16, 2; u32, i32);
impl_primative!(i16, 2; i32, i32);

impl_primative!(u16, 2; u32, u32);
impl_primative!(u16, 2; i32, u32);

impl_primative!(u24, 3; u64, u64);
impl_primative!(u24, 3; i64, u64);

impl_primative!(i24, 3; u64, i64);
impl_primative!(i24, 3; i64, i64);

impl_primative!(i16, 2; u64, i64);
impl_primative!(i16, 2; i64, i64);

impl_primative!(u16, 2; u64, u64);
impl_primative!(u16, 2; i64, u64);

pub trait Sortable: Ord + Sized + Copy {
    fn byte_at<const N: usize>(self) -> u8;
    const ZERO: Option<Self> = None;
}

pub trait SortableBy<I: Sortable>: Copy {
    fn item(self) -> I;
}

macro_rules! impl_sortable_by {
    ($signed:ty, $unsigned:ty) => {
        impl Sortable for $unsigned {
            fn byte_at<const N: usize>(self) -> u8 {
                ((self >> N) & 0xff) as u8
            }
        }

        impl Sortable for $signed {
            fn byte_at<const N: usize>(self) -> u8 {
                (((self as $unsigned) >> N) & 0xff) as u8
            }
            const ZERO: Option<Self> = Some(0);
        }
    };
}

impl_sortable_by!(i16, u16);
impl_sortable_by!(i32, u32);
impl_sortable_by!(i64, u64);

impl<T: Sortable + Copy> SortableBy<T> for T {
    fn item(self) -> T {
        self.clone()
    }
}

impl<I: Sortable, T1: Copy> SortableBy<I> for (I, T1) {
    fn item(self) -> I {
        self.0
    }
}
impl<I: Sortable, T1: Copy, T2: Copy> SortableBy<I> for (I, T1, T2) {
    fn item(self) -> I {
        self.0
    }
}
impl<I: Sortable, T1: Copy, T2: Copy, T3: Copy> SortableBy<I> for (I, T1, T2, T3) {
    fn item(self) -> I {
        self.0
    }
}

#[inline]
fn msd_radix_sort<const OFFSET: usize, I: Sortable, T: SortableBy<I>>(
    source: &[T],
    dest: &mut [T],
    mut count: [usize; 0x100],
) {
    for &s in source.iter().rev() {
        let byte_index = s.item().byte_at::<OFFSET>();
        let e = &mut count[byte_index as usize];
        let index = *e as usize;
        *unsafe { dest.get_unchecked_mut(index) } = s;
        *e = e.wrapping_sub(1);
    }
}

#[inline]
fn msd_radix_sort_by<const OFFSET: usize, I: Sortable, T: Copy>(
    source: &[T],
    dest: &mut [T],
    mut count: [usize; 0x100],
    f: impl Fn(T) -> I,
) {
    for &s in source.iter().rev() {
        let byte_index = f(s).byte_at::<OFFSET>();
        let e = &mut count[byte_index as usize];
        let index = *e as usize;
        *unsafe { dest.get_unchecked_mut(index) } = s;
        *e = e.wrapping_sub(1);
    }
}

pub trait SortableByVec<T> {
    fn sort_fast_by<const N: usize, I: Sortable>(&mut self, f: impl Fn(T) -> I) {
        self.sort_fast_by_reuse::<N, I>(&mut vec![], f);
    }
    fn sort_fast_by_reuse<const N: usize, I: Sortable>(
        &mut self,
        tmp: &mut Vec<T>,
        f: impl Fn(T) -> I,
    );
}

impl<T: Copy> SortableByVec<T> for Vec<T> {
    fn sort_fast_by_reuse<const N: usize, I: Sortable>(
        &mut self,
        tmp: &mut Vec<T>,
        f: impl Fn(T) -> I,
    ) {
        radix_sort_by::<N, T, I>(self, tmp, f);
    }
}

#[cfg(all(not(test), not(debug_assertions)))]
const CUTOFF: usize = 100;

#[cfg(not(all(not(test), not(debug_assertions))))]
const CUTOFF: usize = 0;

pub fn radix_sort_by<const N: usize, T: Copy, I: Sortable>(
    items: &mut Vec<T>,
    tmp: &mut Vec<T>,
    f: impl Fn(T) -> I,
) {
    assert!(N <= 8);
    if items.len() < CUTOFF {
        items.sort_unstable_by(|&a, &b| f(a).cmp(&f(b)));
        return;
    }
    tmp.clear();
    tmp.reserve(items.len());
    unsafe { tmp.set_len(items.len()) };

    const LENGTH: usize = 0x100;
    let mut count: [[usize; LENGTH]; N] = [[0; LENGTH]; N];

    for &s in items.iter() {
        let i = f(s);
        if N >= 1 {
            count[0][i.byte_at::<0>() as usize] += 1;
        }
        if N >= 2 {
            count[1][i.byte_at::<8>() as usize] += 1;
        }
        if N >= 3 {
            count[2][i.byte_at::<16>() as usize] += 1;
        }
        if N >= 4 {
            count[3][i.byte_at::<24>() as usize] += 1;
        }
        if N >= 5 {
            count[4][i.byte_at::<32>() as usize] += 1;
        }
        if N >= 6 {
            count[5][i.byte_at::<40>() as usize] += 1;
        }
        if N >= 7 {
            count[6][i.byte_at::<48>() as usize] += 1;
        }
        if N >= 8 {
            count[7][i.byte_at::<56>() as usize] += 1;
        }
    }

    for j in 0..N {
        for i in 1..LENGTH {
            count[j][i] += count[j][i - 1];
        }
        for i in 0..LENGTH {
            count[j][i] = count[j][i].wrapping_sub(1);
        }
    }

    if N >= 1 {
        msd_radix_sort_by::<0, I, T>(items, tmp, count[0], &f);
    }
    if N >= 2 {
        msd_radix_sort_by::<8, I, T>(tmp, items, count[1], &f);
    }
    if N >= 3 {
        msd_radix_sort_by::<16, I, T>(items, tmp, count[2], &f);
    }
    if N >= 4 {
        msd_radix_sort_by::<24, I, T>(tmp, items, count[3], &f);
    }
    if N >= 5 {
        msd_radix_sort_by::<32, I, T>(items, tmp, count[4], &f);
    }
    if N >= 6 {
        msd_radix_sort_by::<40, I, T>(tmp, items, count[5], &f);
    }
    if N >= 7 {
        msd_radix_sort_by::<48, I, T>(items, tmp, count[6], &f);
    }
    if N >= 8 {
        msd_radix_sort_by::<56, I, T>(tmp, items, count[7], &f);
    }
    if N % 2 == 1 {
        swap(items, tmp);
    }

    if let Some(zero) = I::ZERO {
        let low_items = items.partition_point(|&x| f(x) > zero);
        items.rotate_left(low_items);
    }

    tmp.clear();
}

pub fn radix_sort<const N: usize, I: Sortable, T: SortableBy<I>>(
    items: &mut Vec<T>,
    tmp: &mut Vec<T>,
) {
    if items.len() < CUTOFF {
        items.sort_unstable_by(|a, b| a.item().cmp(&b.item()));
        return;
    }
    tmp.clear();
    tmp.reserve(items.len());
    unsafe { tmp.set_len(items.len()) };

    const LENGTH: usize = 0x100;
    let mut count: [[usize; LENGTH]; N] = [[0; LENGTH]; N];

    for &s in items.iter() {
        let i = s.item();
        if N >= 1 {
            count[0][i.byte_at::<0>() as usize] += 1;
        }
        if N >= 2 {
            count[1][i.byte_at::<8>() as usize] += 1;
        }
        if N >= 3 {
            count[2][i.byte_at::<16>() as usize] += 1;
        }
        if N >= 4 {
            count[3][i.byte_at::<24>() as usize] += 1;
        }
        if N >= 5 {
            count[4][i.byte_at::<32>() as usize] += 1;
        }
        if N >= 6 {
            count[5][i.byte_at::<40>() as usize] += 1;
        }
        if N >= 7 {
            count[6][i.byte_at::<48>() as usize] += 1;
        }
        if N >= 8 {
            count[7][i.byte_at::<56>() as usize] += 1;
        }
    }

    for j in 0..N {
        for i in 1..LENGTH {
            count[j][i] += count[j][i - 1];
        }
        for i in 0..LENGTH {
            count[j][i] = count[j][i].wrapping_sub(1);
        }
    }

    assert!(N <= 8);
    if N >= 1 {
        msd_radix_sort::<0, I, T>(items, tmp, count[0]);
    }
    if N >= 2 {
        msd_radix_sort::<8, I, T>(tmp, items, count[1]);
    }
    if N >= 3 {
        msd_radix_sort::<16, I, T>(items, tmp, count[2]);
    }
    if N >= 4 {
        msd_radix_sort::<24, I, T>(tmp, items, count[3]);
    }
    if N >= 5 {
        msd_radix_sort::<32, I, T>(items, tmp, count[4]);
    }
    if N >= 6 {
        msd_radix_sort::<40, I, T>(tmp, items, count[5]);
    }
    if N >= 7 {
        msd_radix_sort::<48, I, T>(items, tmp, count[6]);
    }
    if N >= 8 {
        msd_radix_sort::<56, I, T>(tmp, items, count[7]);
    }
    if N % 2 == 1 {
        swap(items, tmp);
    }

    if let Some(zero) = I::ZERO {
        let low_items = items.partition_point(|x| x.item() > zero);
        items.rotate_left(low_items);
    }

    tmp.clear();
}

#[allow(non_camel_case_types)]
#[repr(transparent)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct i24(i32);

#[repr(transparent)]
#[allow(non_camel_case_types)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct u24(u32);
/* ==================================================================================================== radix_sort.rs ==================================================================================================== */

fn op(mut positions: [u32; 5], guards: &[Vec<u32>], mut t: u32) -> (u32, [u32; 5]) {
    loop {
        let mut best = u32::MAX;
        let mut best_i = 5;
        for i in 0..5 {
            let Some(&g) = guards[i].get(positions[i] as usize) else {
                continue;
            };
            let value = g * MUL[i];
            if value < best {
                best_i = i;
                best = value;
            }
        }
        if best_i >= 5 {
            break;
        }
        let e = &mut positions[best_i as usize];
        let Some(next_t) = t.checked_sub(guards[best_i][*e as usize]) else {
            break;
        };
        t = next_t;
        *e += 1;
    }

    (t, positions)
}

fn score(x: [u32; 5]) -> u32 {
    let mut sum = 0;
    for i in 0..5 {
        sum += (i + 1) as u32 * x[i];
    }
    sum
}

fn main() {
    //let _m = Times50::new(); // 0.32s, of which 0.29s is io and sort, so 0.00s is kinda out of reach afaik

    let mut io = Io::stdin();
    let (n, t) = (io.next::<u32>(), io.nextln::<u32>());
    let mut gold = [vec![], vec![], vec![], vec![], vec![], vec![]];

    for _ in 0..n {
        let v = io.next_fast::<1, usize>();
        gold[v].push(io.nextln_fast::<7, u32>());
    }
    let [_, mut guards @ ..] = gold;
    let mut tmp = vec![];
    for g in &mut guards {
        g.sort_fast_reuse::<u24>(&mut tmp);
    }

    let (start_t, start_position) = op([0; 5], &guards, t);

    // Brute force the last part
    let mut copy = start_position.clone();
    let mut add = 0;
    for sell_i in 0..5 {
        let count = start_position[sell_i];
        let rem = (5).min(count);
        copy[sell_i] -= rem;
        add += guards[sell_i][(count - rem) as usize..count as usize]
            .iter()
            .sum::<u32>();
    }
    
    const SIZE : usize = 10;
    let mut s0 = copy;
    let mut t0 = start_t + add;
    let mut best_score = score(s0);
    for _ in 0..SIZE {
        let mut t1 = t0;
        let mut s1 = s0;
        for _ in 0..SIZE {
            let mut t2 = t1;
            let mut s2 = s1;
            for _ in 0..SIZE {
                let mut t3 = t2;
                let mut s3 = s2;
                for _ in 0..SIZE {
                    let mut t4 = t3;
                    let mut s4 = s3;
                    for _ in 0..SIZE {
                        let e = &mut s4[4];
                        let Some(&rem) = guards[4].get(*e as usize) else {
                            break;
                        };
                        let Some(new_t) = t4.checked_sub(rem) else {
                            break;
                        };
                        t4 = new_t;
                        *e += 1;
                        best_score = best_score.max(score(s4));
                    }

                    let e = &mut s3[3];
                    let Some(&rem) = guards[3].get(*e as usize) else {
                        break;
                    };
                    let Some(new_t) = t3.checked_sub(rem) else {
                        break;
                    };
                    t3 = new_t;
                    *e += 1;
                    best_score = best_score.max(score(s3));
                }

                let e = &mut s2[2];
                let Some(&rem) = guards[2].get(*e as usize) else {
                    break;
                };
                let Some(new_t) = t2.checked_sub(rem) else {
                    break;
                };
                t2 = new_t;
                *e += 1;
                best_score = best_score.max(score(s2));
            }

            let e = &mut s1[1];
            let Some(&rem) = guards[1].get(*e as usize) else {
                break;
            };
            let Some(new_t) = t1.checked_sub(rem) else {
                break;
            };
            t1 = new_t;
            *e += 1;
            best_score = best_score.max(score(s1));
        }

        let e = &mut s0[0];
        let Some(&rem) = guards[0].get(*e as usize) else {
            break;
        };
        let Some(new_t) = t0.checked_sub(rem) else {
            break;
        };
        t0 = new_t;
        *e += 1;
        best_score = best_score.max(score(s0));
    }

    io.write(best_score);
}
