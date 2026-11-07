#!/bin/bash
# Clone the Athrill core next to this repository unless it is already there.
# The core is pinned to the commit of the toppers/athrill tag v3.1.0.
# ATHRILL_REF / ATHRILL_COMMIT select another tag or branch and its commit.
set -eu

repo_dir="$(cd "$(dirname "$0")/.." && pwd)"
athrill_dir="$(dirname "$repo_dir")/athrill"
athrill_ref="${ATHRILL_REF:-v3.1.0}"
athrill_commit="${ATHRILL_COMMIT:-ed4edd6e7dbcc624dd7bce4d19b471ab27db2ffa}"

if [ -d "$athrill_dir/.git" ]; then
    echo "Athrill core found: $athrill_dir"
    head="$(git -C "$athrill_dir" rev-parse HEAD)"
    if [ "$head" != "$athrill_commit" ]; then
        echo "Warning: $athrill_dir is at $head, not $athrill_ref ($athrill_commit)." >&2
    fi
else
    git -c advice.detachedHead=false clone --branch "$athrill_ref" https://github.com/toppers/athrill.git "$athrill_dir"
    git -C "$athrill_dir" checkout --quiet --detach "$athrill_commit"
    echo "Athrill core: $athrill_ref @ $athrill_commit"
fi
