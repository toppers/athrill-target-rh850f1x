#!/bin/bash
# Clone the Athrill core next to this repository unless it is already there.
# ATHRILL_REF selects the branch (default: cpp).
set -eu

repo_dir="$(cd "$(dirname "$0")/.." && pwd)"
athrill_dir="$(dirname "$repo_dir")/athrill"
athrill_ref="${ATHRILL_REF:-cpp}"

if [ -d "$athrill_dir/.git" ]; then
    echo "Athrill core found: $athrill_dir"
else
    git clone --branch "$athrill_ref" https://github.com/toppers/athrill.git "$athrill_dir"
fi
