# KB38 — ZMK Firmware

ZMK firmware for a monolithic split keyboard with 38 keys and Cirque trackpad,
running on nice!nano v2 (SuperMini NRF52840 is pin-compatible).

## Repository Layout

```
├── build.sh                              Build script (Docker)
├── build.yaml                            CI build config
├── wiring.md                             Pin assignments and wiring
├── config/
│   ├── west.yml                          ZMK manifest
│   ├── kb38.keymap                       Layers and behaviors
│   └── boards/shields/kb38/
│       ├── Kconfig.shield
│       ├── Kconfig.defconfig
│       ├── kb38.conf                     Kconfig (Cirque, BLE, sleep)
│       └── kb38.overlay                  Device tree (matrix, I2C, trackpad)
└── .github/workflows/build.yml           GitHub Actions CI
```

## Hardware

| Item | Value |
|------|-------|
| Controller | nice!nano v2 (SuperMini NRF52840 pin-compatible) |
| Matrix | 4 rows x 10 cols, diode col-to-row |
| Physical keys | 38 (RC(3,0) and RC(3,9) absent) |
| Trackpad | Cirque TM035035-2024-003 — I2C, addr 0x2A |
| I2C pins | SDA = P1.15 (D18), SCL = P0.02 (D19) via I2C1 |
| DR pin | P0.17 (D2) — data-ready interrupt |
| Battery | KMD 402030 — 3.7V 200mAh LiPo |
| Power switch | SPST rocker, series with battery + → RAW |
| Voltage indicator | 1S LiPo LED bar with TEST button, wired before switch |

Full wiring: see `wiring.md`.

## Key Positions

```
 [ 0][ 1][ 2][ 3][ 4]   [ 5][ 6][ 7][ 8][ 9]   row 0
 [10][11][12][13][14]   [15][16][17][18][19]   row 1
 [20][21][22][23][24]   [25][26][27][28][29]   row 2
     [30][31][32][33]   [34][35][36][37]       row 3

 30 = LCLK          31 = Caps Word
 32 = Tab / SYM     33 = Space / NAV
 34 = Enter / MEDIA 35 = Bspc / NUM
 36 = Caps Word     37 = RCLK
```

## Layers

| # | Name | Activation |
|---|------|------------|
| 0 | BASE | Default |
| 1 | SYM | Tab hold (32) |
| 2 | EMPTY | Do not use (firmware bug workaround) |
| 3 | NAV | Space hold (33) |
| 4 | NUM | Bspc hold (35) |
| 5 | MEDIA | Enter hold (34) |
| 6 | SCROLL | G hold on NAV (Space + G) — trackpad becomes scroll |

## Behaviors

- **hml/hmr** — positional home-row mods, ACGS order (220ms term, 150ms quick-tap, 180ms idle)
- **hml_shift/hmr_shift** — shift variants on F/J (125ms idle for eager activation)
- **hyp** — Hyper mod (all four modifiers), triggers on both hands
- **lt_repeat** — layer-tap with key-repeat (tap-tap-hold repeats)

## Combos

| Keys | Positions | Output | Layer |
|------|-----------|--------|-------|
| S + D | 11, 12 | ESC | BASE |
| K + L | 17, 18 | ESC | BASE |

## Trackpad Notes

- Sensitivity must be `"2x"` or lower. Higher values cause erratic cursor.
- Y-axis is inverted via `zmk,input-processor-transform`.
- Scroll mode: hold Space + G, then slide trackpad.
- `CONFIG_INPUT_INIT_PRIORITY=99` is required — Cirque needs ~300ms power-on reset.
- `CONFIG_I2C=y`, `CONFIG_INPUT=y`, `CONFIG_INPUT_PINNACLE=y` must be explicit in `.conf`.

## Common Tasks

### Change a key binding
Edit `config/kb38.keymap`. Find the layer, count positions from the layout above.

### Adjust home-row mod timing
In `kb38.keymap`, modify `tapping-term-ms` in the `hml`/`hmr` behavior blocks.

### Change Cirque sensitivity
In `kb38.overlay`, find `pinnacle@2a` and change `sensitivity`.
Valid: `"1x"`, `"2x"`. Higher values are unstable.

### Build
```bash
./build.sh              # build firmware
./build.sh reset        # settings_reset firmware
./build.sh --flash      # build and flash
```
