suppfile () {
    local DIR; DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null && pwd)"
    local suppfilepath="$DIR"/leaks.supp
    echo "$suppfilepath"
}

LSAN_OPTIONS=suppressions="$(suppfile)",print_suppressions=0
export LSAN_OPTIONS
