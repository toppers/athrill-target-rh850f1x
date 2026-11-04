#!/bin/sh
# Set the target version everywhere it is written.
#
# usage: tools/set-version.sh <X.Y.Z>
#   Updates CMakeLists.txt (project VERSION, the source of truth),
#   MANIFEST (VERSION, read by makerelease) and the fallback
#   ATHRILL_TARGET_VERSION in src/cpu/target_cpu.h, then runs
#   tools/check-version.sh against v<X.Y.Z>.
set -eu

[ "$#" -eq 1 ] || { echo "usage: $0 <X.Y.Z>" >&2; exit 2; }
version=$1
case "$version" in
    *[!0-9.]*|.*|*.|*..*) echo "error: '$version' is not X.Y.Z" >&2; exit 2 ;;
    [0-9]*.[0-9]*.[0-9]*) ;;
    *) echo "error: '$version' is not X.Y.Z" >&2; exit 2 ;;
esac
[ "$(printf '%s' "$version" | tr -cd . | wc -c)" -eq 2 ] || {
    echo "error: '$version' is not X.Y.Z" >&2; exit 2; }

cd "$(dirname "$0")/.."

replace()
{
    file=$1
    expr=$2
    sed "$expr" "$file" > "$file.tmp"
    if cmp -s "$file" "$file.tmp"; then
        rm -f "$file.tmp"
    else
        mv "$file.tmp" "$file"
    fi
}

replace CMakeLists.txt "s/^project(athrill3 VERSION [^ )]*/project(athrill3 VERSION $version/"
replace MANIFEST "s/^VERSION[ 	].*/VERSION $version/"
replace src/cpu/target_cpu.h "s/^#define ATHRILL_TARGET_VERSION \".*\"$/#define ATHRILL_TARGET_VERSION \"$version\"/"

sh tools/check-version.sh "v$version"
