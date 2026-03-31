#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$ROOT_DIR/.wsl_build"
RUN_DIR="$BUILD_DIR/run"
OUT_DIR="${1:-$ROOT_DIR/tests}"

declare -A TESTS_PER_SUBTASK=(
    [1]=3
    [2]=4
    [3]=5
    [4]=3
    [5]=4
    [6]=5
    [7]=3
    [8]=4
    [9]=5
    [10]=5
)

variant_for_subtask() {
    local subtask="$1"
    if (( subtask >= 1 && subtask <= 3 )); then
        echo 1
    elif (( subtask >= 4 && subtask <= 6 )); then
        echo 2
    else
        echo 3
    fi
}

generator_mode_for_subtask() {
    local subtask="$1"
    local test_id="$2"
    local count="$3"
    if (( subtask == 10 )); then
        echo "distinct"
    elif (( subtask != 9 )); then
        echo ""
    elif (( test_id <= count / 2 )); then
        echo "distinct"
    else
        echo "centered"
    fi
}

compile_generator_for_subtask() {
    local subtask="$1"
    local src="$BUILD_DIR/generator_st${subtask}.cpp"
    local bin="$BUILD_DIR/generator_st${subtask}"

    perl -0pe "s/\\bint C\\s*=\\s*\\d+\\s*;/int C = ${subtask};/" \
        "$ROOT_DIR/generator.cpp" > "$src"

    g++ -std=c++17 -O2 "$src" -o "$bin"
    echo "$bin"
}

fix_input_header() {
    local variant="$1"
    {
        echo "$variant"
        tail -n +2 "$RUN_DIR/polen.in"
    } > "$RUN_DIR/polen.fixed"
    mv "$RUN_DIR/polen.fixed" "$RUN_DIR/polen.in"
}

mkdir -p "$BUILD_DIR" "$RUN_DIR" "$OUT_DIR"

MAIN_BIN="$BUILD_DIR/main_solver"
g++ -std=c++17 -O2 "$ROOT_DIR/main.cpp" -o "$MAIN_BIN"

global_test_id=1

for subtask in 1 2 3 4 5 6 7 8 9 10; do
    generator_bin="$(compile_generator_for_subtask "$subtask")"
    variant="$(variant_for_subtask "$subtask")"

    count="${TESTS_PER_SUBTASK[$subtask]}"
    for ((test_id = 1; test_id <= count; test_id++)); do
        rm -f "$RUN_DIR/polen.in" "$RUN_DIR/polen.out"
        gen_mode="$(generator_mode_for_subtask "$subtask" "$test_id" "$count")"
        safe_int_mode=""
        if (( test_id <= 2 )); then
            safe_int_mode="1"
        fi

        (
            cd "$RUN_DIR"
            if [[ -n "$gen_mode" && -n "$safe_int_mode" ]]; then
                POLEN_C3_MODE="$gen_mode" POLEN_SAFE_INT="$safe_int_mode" "$generator_bin"
            elif [[ -n "$gen_mode" ]]; then
                POLEN_C3_MODE="$gen_mode" "$generator_bin"
            elif [[ -n "$safe_int_mode" ]]; then
                POLEN_SAFE_INT="$safe_int_mode" "$generator_bin"
            else
                "$generator_bin"
            fi
        )

        fix_input_header "$variant"

        (
            cd "$RUN_DIR"
            "$MAIN_BIN"
        )

        printf -v idx "%02d" "$global_test_id"
        cp "$RUN_DIR/polen.in" "$OUT_DIR/input.test${idx}"
        cp "$RUN_DIR/polen.out" "$OUT_DIR/output.test${idx}"
        ((global_test_id++))
    done
done

echo "Testele au fost generate in: $OUT_DIR"
