# athrill-target-rh850f1x

RH850F1x target for [Athrill](https://github.com/toppers/athrill).

## Build

Check out this repository beside the `toppers/athrill` core checkout (branch `cpp`):

```sh
git clone -b cpp https://github.com/toppers/athrill.git
git clone https://github.com/toppers/athrill-target-rh850f1x.git
cd athrill-target-rh850f1x
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

Set `ATHRILL_SOURCE_DIR` if the core is elsewhere. The Business Pack build entry point is `python3 tools/hako.py doctor`, `build`, and `smoke`; settings come from `hakoniwa-build.yaml`. Install with `cmake --install build --prefix .` to produce `bin/athrill3` for packaging.

Hakoniwa CAN transport is optional and defaults off. Enable it with `-DRH850_ENABLE_HAKO_CAN=ON -DATHRILL_ENABLE_HAKONIWA=ON`; the core's Hakoniwa PDU definition must provision all configured CAN channels. `HAKONIWA_PDU_REGISTRY_DIR` selects the sibling PDU registry checkout.

## License

Distributed under the [TOPPERS License](https://toppers.jp/en/license.html).
