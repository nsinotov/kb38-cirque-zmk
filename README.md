# KB38 — ZMK Firmware

Monolithic split keyboard (38 keys + Cirque trackpad) on **nice!nano v2**.
SuperMini NRF52840 is pin-compatible and works as a drop-in replacement.

Adapted from [nsinotov/urchin-zmk-firmware](https://github.com/nsinotov/urchin-zmk-firmware).

## Hardware

| Component | Spec |
|-----------|------|
| Controller | nice!nano v2 (nRF52840) |
| Matrix | 4 rows x 10 cols, 38 physical keys |
| Trackpad | Cirque TM035035 — I2C, addr 0x2A |
| Battery | 3.7V LiPo 1S (recommended: 301230 ~100 mAh) |
| BLE name | KB38-Cirque |

See [wiring.md](wiring.md) for full pin assignment and wiring diagram.

## Layout

Legend: `S`=Shift `C`=Ctrl `A`=Alt `G`=GUI `H`=Hyper(GCAS) · `dn`=layer-tap to layer n

---

### Layer 0 — BASE (QWERTY)

```
┌─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┐
│  Q  │  W  │  E  │  R  │  T  │       │  Y  │  U  │  I  │  O  │  P  │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│ A/S │ S/C │ D/A │ F/G │ G/H │       │ H/H │ J/G │ K/A │ L/C │ ;/S │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│  Z  │  X  │  C  │  V  │  B  │       │  N  │  M  │  ,  │  .  │  /  │
└─────┼─────┼─────┼─────┼─────┘       └─────┼─────┼─────┼─────┼─────┘
      │LCLK │ CW  │Tabd1│Spcd3│       │Entd5│Bspd4│ CW  │RCLK │
      └─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┘
```

> LCLK/RCLK = mouse buttons · CW = Caps Word · Home-row: hold for modifier, tap for key

---

### Layer 1 — SYM (Symbols) — Tab hold

```
┌─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┐
│  !  │  @  │  #  │  $  │  %  │       │  ^  │  &  │  *  │  (  │  )  │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│ ./S │ ,/C │ "/A │ '/G │  |  │       │  -  │ =/G │ `/A │ {/C │ }/S │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│  ?  │  /  │  <  │  >  │  \  │       │  _  │  +  │  ~  │  [  │  ]  │
└─────┼─────┼─────┼─────┼─────┘       └─────┼─────┼─────┼─────┼─────┘
      │LCLK │     │     │     │       │ Ent │ Del │     │RCLK │
      └─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┘
```

---

### Layer 2 — EMPTY *(firmware bug workaround — do not use)*

---

### Layer 3 — NAV (Navigation) — Space hold

```
┌─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┐
│CAPS │     │     │     │     │       │HOME │PgDn │PgUp │ END │ Del │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│  S  │  C  │  A  │  G  │SCRL │       │  <  │  v  │  ^  │  >  │Bspc │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│     │     │ G-C │ G-V │     │       │     │     │     │     │     │
└─────┼─────┼─────┼─────┼─────┘       └─────┼─────┼─────┼─────┼─────┘
      │LCLK │     │     │     │       │ Ent │ Del │     │RCLK │
      └─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┘
```

> SCRL = hold to scroll with trackpad (Space + G)

---

### Layer 4 — NUM (Numbers & F-keys) — Backspace hold

```
┌─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┐
│ F1  │ F2  │ F3  │ F4  │ F5  │       │ F6  │ F7  │ F8  │ F9  │F10  │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│ 1/S │ 2/C │ 3/A │ 4/G │  5  │       │  6  │ 7/G │ 8/A │ 9/C │ 0/S │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│F11  │     │     │     │     │       │     │     │     │     │F12  │
└─────┼─────┼─────┼─────┼─────┘       └─────┼─────┼─────┼─────┼─────┘
      │LCLK │     │ Tab │ Spc │       │     │     │     │RCLK │
      └─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┘
```

---

### Layer 5 — MEDIA (Bluetooth & System) — Enter hold

```
┌─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┐
│ BT0 │     │ |<< │ >>| │ >|| │       │ BT3 │     │     │UNSTK│ RST │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│ BT1 │     │VOL- │VOL+ │MUTE │       │ BT4 │     │     │     │BOOT │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│ BT2 │     │BRI- │BRI+ │     │       │ BT5 │     │     │     │BTCLR│
└─────┼─────┼─────┼─────┼─────┘       └─────┼─────┼─────┼─────┼─────┘
      │LCLK │     │     │     │       │     │     │     │RCLK │
      └─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┘
```

---

### Layer 6 — SCROLL (trackpad scroll mode)

Activated by holding G on the NAV layer (Space + G). All keys pass through
to the NAV layer. Trackpad Y-movement becomes vertical scroll, X-movement
becomes horizontal scroll.

---

### Combos

| Keys | Layer | Output |
|------|-------|--------|
| S + D | BASE | ESC |
| K + L | BASE | ESC |

## Building

### GitHub Actions

Push to `main`. Download `kb38.uf2` from Actions > latest run > Artifacts.

### Local build (Docker)

```bash
./build.sh              # build firmware
./build.sh reset        # build settings_reset firmware
./build.sh --flash      # build and flash to bootloader drive
```

### Flashing

1. Double-tap RST to enter bootloader (drive named `NICENANO` appears)
2. Copy `kb38.uf2` to the drive, or use `./build.sh --flash`

## SuperMini NRF52840

Pin-compatible drop-in replacement for nice!nano v2. Uses the same
`nice_nano//zmk` board target. Known differences:

- LED colors are swapped (blue = BT, red = charging)
- Battery voltage divider (P0.24) is unpopulated by default
- Sleep current is higher (~700 uA vs ~20 uA on genuine nice!nano)

## Known Issues

- **Layer 2 is intentionally empty.** ZMK firmware bug: layer index 2 + key
  position 2 causes BLE disconnect. Do not assign bindings to layer 2.
- **Sensitivity above 2x causes erratic cursor.** With `"3x"` or `"4x"`, the
  cursor flies to one corner instead of tracking finger movement. Keep at `"2x"`.

## License

MIT
