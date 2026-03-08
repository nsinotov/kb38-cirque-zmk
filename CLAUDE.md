# KB38 — ZMK Firmware · Claude Context

## Project

ZMK firmware for a **monolithic split keyboard** with 38 keys + Cirque trackpad,
running on **nice!nano v2** (or SuperMini NRF52840 clone).

Adapted from [nsinotov/urchin-zmk-firmware](https://github.com/nsinotov/urchin-zmk-firmware).

---

## Repository layout

```
.
├── build.yaml                              ← CI: board + shield pair
├── build.sh                                ← local build helper
├── wiring.md                               ← hardware pin assignment
├── config/
│   ├── west.yml                            ← ZMK manifest (pins ZMK revision)
│   ├── kb38.keymap                         ← all layers and behaviors
│   └── boards/shields/kb38/
│       ├── Kconfig.shield
│       ├── Kconfig.defconfig
│       ├── kb38.conf                       ← Kconfig options (Cirque, BLE, sleep)
│       └── kb38.dtsi                       ← devicetree: matrix + I2C/Cirque
└── .github/workflows/build.yml             ← GitHub Actions CI
```

---

## Hardware summary

| Item | Value |
|------|-------|
| Controller | nice!nano v2 / SuperMini NRF52840 |
| Matrix | 4 rows × 10 cols, diode col-to-row |
| Physical keys | 38 (RC(3,0) and RC(3,9) absent) |
| Trackpad | Cirque TM035035-2024-003 — I2C, addr 0x2A |
| I2C pins | SDA = D18 (P0.28) · SCL = D19 (P0.29) via i2c1 |
| Battery | 3.7V LiPo 1S via RAW pin |

Full wiring: `wiring.md`.

---

## Key layout (indices)

```
 [ 0][ 1][ 2][ 3][ 4]   [ 5][ 6][ 7][ 8][ 9]   ← row 0
 [10][11][12][13][14]   [15][16][17][18][19]   ← row 1
 [20][21][22][23][24]   [25][26][27][28][29]   ← row 2
     [30][31][32][33]   [34][35][36][37]       ← row 3

 30 = LCLK (left mouse)   31 = Caps Word
 32 = Tab / SYM layer     33 = Space / NAV layer
 34 = Enter / MEDIA layer 35 = Bspc / NUM layer
 36 = Caps Word           37 = RCLK (right mouse)
```

---

## Layers

| # | Name | Activated by |
|---|------|--------------|
| 0 | BASE | — (default) |
| 1 | SYM | Tab hold (key 32) |
| 2 | EMPTY | — (intentionally blank, firmware bug workaround) |
| 3 | NAV | Space hold (key 33) |
| 4 | NUM | Bspc hold (key 35) |
| 5 | MEDIA | Enter hold (key 34) |

> **Do not add bindings to layer 2.** ZMK bug: layer index 2 + key position 2
> causes BLE disconnect. Keep layer 2 empty as a placeholder.

---

## Behaviors

### `hm` — home-row mod-tap
- `tapping-term-ms = 220` · `quick-tap-ms = 250` · `require-prior-idle-ms = 125`
- flavor: `balanced`
- Used on all home-row keys (A/S/D/F/G left, H/J/K/L/; right)

### `lt_repeat` — layer-tap with key-repeat
- `tapping-term-ms = 200` · `quick-tap-ms = 200`
- flavor: `balanced`
- Single tap → key; hold → layer; tap-tap-hold → key repeat
- Used on all four thumb cluster keys (32–35)

---

## Combos

| Keys | Positions | Output | Layer |
|------|-----------|--------|-------|
| S + D | 11, 12 | ESC | BASE |
| K + L | 17, 18 | ESC | BASE |

---

## Common tasks

### Change a key binding
Edit `config/kb38.keymap`. Find the layer, count key positions from the
matrix transform in `kb38.dtsi`. Rebuild.

### Adjust home-row mod timing
In `kb38.keymap`, find the `hm: homerow_mods` behavior block.
Increase `tapping-term-ms` if accidental mod triggers; decrease for faster mods.

### Add a new combo
In `kb38.keymap`, add a block inside `combos { }`:
```dts
combo_name {
    timeout-ms = <50>;
    key-positions = <A B>;   /* indices from the layout above */
    bindings = <&kp KEYCODE>;
    layers = <BASE>;
};
```

### Change Cirque sensitivity
In `kb38.dtsi`, find `pinnacle@2a` and change `sensitivity`:
valid values: `"1x"` `"2x"` `"3x"` `"4x"` (default: `"4x"`).

### Disable Cirque (debug matrix without trackpad)
In `kb38.conf`, add:
```
CONFIG_ZMK_POINTING=n
CONFIG_CIRQUE_PINNACLE=n
```

### Enable battery reporting (SuperMini with voltage divider populated)
In `kb38.conf`, ensure:
```
CONFIG_ZMK_BATTERY_REPORTING=y
```
The voltage divider on SuperMini boards is unpopulated by default (P0.24 / D6).
If not populated, disable this or battery % will always show 0.

### Local build
```bash
# First time only:
west init -l config
west update

# Every build:
./build.sh             # incremental
./build.sh --clean     # pristine
./build.sh --flash     # build + copy to NICENANO bootloader drive (macOS)
```

### GitHub Actions build
Push to `main`. Download `zmk.uf2` from Actions → latest run → Artifacts.

---

## ZMK documentation references

- Behaviors: https://zmk.dev/docs/behaviors
- Combos: https://zmk.dev/docs/features/combos
- Pointing devices (Cirque): https://zmk.dev/docs/features/pointing
- Keycodes: https://zmk.dev/docs/codes
- Board/shield config: https://zmk.dev/docs/development/hardware-integration
