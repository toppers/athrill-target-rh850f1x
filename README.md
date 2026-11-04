# athrill-target-rh850f1x

![build](https://github.com/toppers/athrill-target-rh850f1x/actions/workflows/build.yml/badge.svg?branch=cpp)

RH850F1x target for [Athrill](https://github.com/toppers/athrill).

This `cpp` branch builds `athrill3` with CMake on the `toppers/athrill` core (v3.0.0 or later).
The `master` branch keeps the earlier Makefile-based releases (v0.A.x, `athrill2`).

## Supported targets

- RH850/F1K
    - Reference manuals (Japanese editions):
        - RH850/F1K ユーザーズマニュアル ハードウェア編, Rev.1.10, 2016-12-21 (R01UH0562JJ0110)
        - RH850G3KH ユーザーズマニュアル ソフトウェア編, Rev.1.20, 2016-12-22 (R01US0165JJ0120)
    - The CPU instruction decoder and executor (`src/cpu/cpu_dec`, `src/cpu/cpu_exec`) are based on [athrill-target-v850e2m](https://github.com/toppers/athrill-target-v850e2m), not written from the RH850 manuals.

## Requirements

- CMake 3.25 or later
- A C and C++17 compiler (tested with GCC 13 on Ubuntu 24.04)
- The `toppers/athrill` core v3.0.0 or later (configure warns about an older core)
- Optional: Python 3 for `tools/hako.py`, and hakoniwa-core-pro and hakoniwa-pdu-registry for Hakoniwa CAN

## Build

Check out this repository beside the `toppers/athrill` core checkout (v3.0.0 or later):

```sh
git clone -b v3.0.0 https://github.com/toppers/athrill.git
git clone -b cpp https://github.com/toppers/athrill-target-rh850f1x.git
cd athrill-target-rh850f1x
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

Set `ATHRILL_SOURCE_DIR` if the core is elsewhere. The Business Pack build entry point is `python3 tools/hako.py doctor`, `build`, and `smoke`; settings come from `hakoniwa-build.yaml`. Install with `cmake --install build --prefix .` to produce `bin/athrill3` for packaging.

Hakoniwa CAN transport is optional and defaults off. Enable it with `-DRH850_ENABLE_HAKO_CAN=ON -DATHRILL_ENABLE_HAKONIWA=ON`; the core's Hakoniwa PDU definition must provision all configured CAN channels. `HAKONIWA_PDU_REGISTRY_DIR` selects the sibling PDU registry checkout.

## License

Distributed under the [TOPPERS License](https://toppers.jp/en/license.html).
