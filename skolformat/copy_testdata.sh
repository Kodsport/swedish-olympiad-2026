#!/usr/bin/env bash

# First arg: a folder
# Recursively find all .in and .ans files
# It takes all of them, sorted lexicographically
# and saves them in a folder provided by second argument
# Name for each is {third arg}{index 1-based}.in (or .ans)
# Example usage:
# 
# bash copy_testdata.sh ../skol/binarapalindrom/data/secret/ testdata-HEMLIGT/ binarapalindrom
# 
# which creates files
# testdata-HEMLIGT/binarapalindrom1.in
# testdata-HEMLIGT/binarapalindrom1.ans
# testdata-HEMLIGT/binarapalindrom2.in
# testdata-HEMLIGT/binarapalindrom2.ans
# ...
# Completely written by ChatGPT, but seems to work

set -euo pipefail

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <input_folder> <output_path> <base>"
    exit 1
fi

input_dir="$1"
out_dir="$2"
base="$3"

# Ensure output directory exists
mkdir -p "$out_dir"

# Find all .in and .ans files, sorted lexicographically
mapfile -t files < <(find "$input_dir" -type f \( -name "*.in" -o -name "*.ans" \) | sort)

# Associate arrays to map base names to file paths
declare -A in_files
declare -A ans_files

# Collect .in and .ans into maps
for f in "${files[@]}"; do
    name="$(basename "$f")"
    stem="${name%.*}"
    ext="${name##*.}"
    
    if [[ "$ext" == "in" ]]; then
        in_files["$stem"]="$f"
    elif [[ "$ext" == "ans" ]]; then
        ans_files["$stem"]="$f"
    fi
done

# Index and copy matched pairs
i=1
for stem in $(printf "%s\n" "${!in_files[@]}" | sort); do
    if [[ -n "${ans_files[$stem]+ok}" ]]; then
        cp "${in_files[$stem]}" "$out_dir/${base}${i}.in"
        cp "${ans_files[$stem]}" "$out_dir/${base}${i}.ans"
        ((i++))
    fi
done
