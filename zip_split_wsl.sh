#!/usr/bin/env bash
set -euo pipefail

SRC_DIR="${1:-tests}"
OUT_PREFIX="${2:-$(basename "$SRC_DIR")}"
MAX_MB="${3:-25}"
OUT_PREFIX="${OUT_PREFIX%.zip}"

if [[ ! -d "$SRC_DIR" ]]; then
    echo "Director inexistent: $SRC_DIR" >&2
    exit 1
fi

if ! command -v zip >/dev/null 2>&1; then
    echo "Comanda 'zip' nu este instalata." >&2
    exit 1
fi

if ! [[ "$MAX_MB" =~ ^[0-9]+$ ]] || (( MAX_MB <= 0 )); then
    echo "Limita trebuie sa fie un numar intreg pozitiv de MB." >&2
    exit 1
fi

MAX_BYTES=$((MAX_MB * 1024 * 1024))
CALLER_DIR="$(pwd)"
SRC_DIR_ABS="$(cd "$SRC_DIR" && pwd)"
case "$OUT_PREFIX" in
    /*) OUT_PREFIX_ABS="$OUT_PREFIX" ;;
    *) OUT_PREFIX_ABS="$CALLER_DIR/$OUT_PREFIX" ;;
esac
mkdir -p "$(dirname "$OUT_PREFIX_ABS")"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

mapfile -t files < <(
    cd "$SRC_DIR_ABS"
    find . -type f ! -name '*.zip' | sed 's#^\./##' | sort
)

if (( ${#files[@]} == 0 )); then
    echo "Nu exista fisiere de arhivat in: $SRC_DIR_ABS" >&2
    exit 1
fi

declare -A group_files
groups=()

for rel in "${files[@]}"; do
    base="$(basename "$rel")"
    if [[ "$base" =~ ^(input|output)\.test(.+)$ ]]; then
        key="test${BASH_REMATCH[2]}"
    else
        key="$rel"
    fi

    if [[ -z "${group_files[$key]+x}" ]]; then
        groups+=("$key")
        group_files["$key"]="$rel"
    else
        group_files["$key"]+=$'\n'"$rel"
    fi
done

build_archive() {
    local archive_path="$1"
    shift

    local batch_groups=("$@")
    local archive_files=()
    local key rel

    for key in "${batch_groups[@]}"; do
        while IFS= read -r rel; do
            [[ -z "$rel" ]] && continue
            archive_files+=("$rel")
        done <<< "${group_files[$key]}"
    done

    rm -f "$archive_path"
    (
        cd "$SRC_DIR_ABS"
        zip -q -r "$archive_path" "${archive_files[@]}"
    )
}

archive_name() {
    local part="$1"
    printf '%s_part%02d.zip' "$OUT_PREFIX_ABS" "$part"
}

current_groups=()
part=1

for key in "${groups[@]}"; do
    trial_groups=("${current_groups[@]}" "$key")
    candidate_zip="$TMP_DIR/candidate.zip"

    build_archive "$candidate_zip" "${trial_groups[@]}"
    candidate_size="$(stat -c %s "$candidate_zip")"

    if (( candidate_size <= MAX_BYTES )); then
        current_groups=("${trial_groups[@]}")
        continue
    fi

    if (( ${#current_groups[@]} == 0 )); then
        out_zip="$(archive_name "$part")"
        build_archive "$out_zip" "$key"
        out_size="$(stat -c %s "$out_zip")"
        echo "Creat: $out_zip (${out_size} bytes) [un singur grup depaseste limita]"
        ((part++))
        continue
    fi

    out_zip="$(archive_name "$part")"
    build_archive "$out_zip" "${current_groups[@]}"
    out_size="$(stat -c %s "$out_zip")"
    echo "Creat: $out_zip (${out_size} bytes)"
    ((part++))
    current_groups=("$key")
done

if (( ${#current_groups[@]} > 0 )); then
    out_zip="$(archive_name "$part")"
    build_archive "$out_zip" "${current_groups[@]}"
    out_size="$(stat -c %s "$out_zip")"
    echo "Creat: $out_zip (${out_size} bytes)"
fi

echo "Arhivarea s-a terminat. Limita a fost ${MAX_MB} MB per zip."
