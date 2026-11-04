#!/bin/sh
# Check that a release tag matches the target version written in the tree.
#
# usage: tools/check-version.sh <tag>
#   v1.0.0, RC-v1.0.0 and RC2-v1.0.0 all expect version 1.0.0 in
#   CMakeLists.txt (project VERSION), MANIFEST (VERSION) and
#   src/cpu/target_cpu.h (ATHRILL_TARGET_VERSION).
set -eu

[ "$#" -eq 1 ] || { echo "usage: $0 <tag>" >&2; exit 2; }
tag=$1

cd "$(dirname "$0")/.."

version=${tag#RC*-}
version=${version#v}
case "$version" in
    [0-9]*.[0-9]*.[0-9]*) ;;
    *) echo "error: cannot read a version from tag '$tag'" >&2; exit 1 ;;
esac

cmake_version=$(sed -n 's/^project(athrill3 VERSION \([^ )]*\).*/\1/p' CMakeLists.txt)
manifest_version=$(sed -n 's/^VERSION[ 	]*//p' MANIFEST)
source_version=$(sed -n 's/^#define ATHRILL_TARGET_VERSION "\(.*\)"$/\1/p' src/cpu/target_cpu.h)

status=0
for pair in "CMakeLists.txt:$cmake_version" "MANIFEST:$manifest_version" "src/cpu/target_cpu.h:$source_version"; do
    file=${pair%%:*}
    found=${pair#*:}
    if [ "$found" = "$version" ]; then
        echo "ok    $file: $found"
    else
        echo "error $file: '$found' does not match tag $tag ($version)" >&2
        status=1
    fi
done
exit $status
