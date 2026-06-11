// https://pokval26.kattis.com/contests/pokval26/problems/lojtnantslotto
use crate::{
    fxhash::FxHashSet,
    messure_time::Times50,
    radix_sort::{Sorting, u24},
};
use fast_io_ln::Io;

const MUL: [u32; 5] = [60, 30, 20, 15, 12];

mod fast_io_ln;
mod fxhash;
mod messure_time;
mod radix_sort;

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
    let _m = Times50::new();

    let mut io = Io::stdin();
    let (n, t) = (io.next::<u32>(), io.nextln::<u32>());
    let mut gold = [const { vec![] }; 6];

    for _ in 0..n {
        let v = io.next_fast::<1, usize>();
        unsafe {
            std::hint::assert_unchecked(v < 6);
        }
        gold[v].push(io.nextln::<u32>());
    }
    let [_, mut guards @ ..] = gold;
    let mut tmp = vec![];
    for g in &mut guards {
        g.sort_fast_reuse::<u24>(&mut tmp);
    }

    let (start_t, start_position) = op([0; 5], &guards, t);

    let mut best = start_position;
    let mut best_score = score(best);
    let mut visisted = FxHashSet::default();
    let mut dfs = vec![];

    // Brute force the last part
    let mut copy = start_position.clone();
    let mut add = 0;
    for sell_i in 0..4 {
        let count = start_position[sell_i];
        let rem = (4 - sell_i as u32).min(count);
        copy[sell_i] -= rem;
        add += guards[sell_i][(count - rem) as usize..count as usize]
            .iter()
            .sum::<u32>();
    }

    dfs.push((start_t + add, copy));
    while let Some((t, state)) = dfs.pop() {
        for buy_i in 0..5 {
            let mut state = state;
            let e = &mut state[buy_i as usize];
            let Some(&rem) = guards[buy_i].get(*e as usize) else {
                continue;
            };
            let Some(t) = t.checked_sub(rem) else {
                continue;
            };
            *e += 1;
            if !visisted.insert((t, state)) {
                continue;
            }
            dfs.push((t, state));
            let score = score(state);
            if score > best_score {
                best = state;
                best_score = score;
            }
        }
    }

    io.write(score(best));
}
