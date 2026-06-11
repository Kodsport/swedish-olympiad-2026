/*
 * Author: Vincent Lagerros vinlag@kth.se
 * Date: 2025-02-18
 *
 * This is used for optimizing 0.01s solutions to 0.00s, as you can better messure the time.
 *
 * Note: For kattis you need 0.17s/50 or less then 0.0035s to get 0.00s
 * Note: Kattis only messures the slowest time, not average or sum. So dont bother with fast cases.
 */

use std::time::Instant;

pub type Times100 = MessureTime<100>;
pub type Times50 = MessureTime<50>;
pub type Times25 = MessureTime<25>;
pub type Times10 = MessureTime<10>;

pub struct MessureTime<const MUL: u32> {
    start: Instant,
}

impl<const MUL: u32> MessureTime<MUL> {
    pub fn new() -> Self {
        Self {
            start: Instant::now(),
        }
    }
}

impl<const MUL: u32> Drop for MessureTime<MUL> {
    fn drop(&mut self) {
        #[cfg(target_os = "windows")]
        eprintln!(
            "\n{}ms",
            Instant::now().duration_since(self.start).as_secs_f64() * 1000.0
        );
        // busy wait
        #[cfg(target_os = "linux")]
        {
            let wait_until = self.start + Instant::now().duration_since(self.start) * MUL;
            while Instant::now() < wait_until {}
        }
    }
}
