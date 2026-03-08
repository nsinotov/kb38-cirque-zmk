# KB38 — ZMK Firmware

Monolithic split keyboard (38 keys + Cirque trackpad) on **nice!nano v2** / SuperMini NRF52840.

Adapted from [nsinotov/urchin-zmk-firmware](https://github.com/nsinotov/urchin-zmk-firmware).

## Hardware

| Component | Spec |
|-----------|------|
| Controller | nice!nano v2 (or SuperMini NRF52840) |
| Matrix | 4 rows × 10 cols; 38 physical keys |
| Trackpad | Cirque TM035035 — I2C mode, addr 0x2A |
| Battery | 3.7V LiPo 1S (recommended: 301230 ~100 mAh) |

See [wiring.md](wiring.md) for full pin assignment and wiring diagram.

## Layout

Legend: `⇧`=Shift `⌃`=Ctrl `⌥`=Alt `⌘`=GUI `✦`=Hyper(⌘⌥⌃⇧) · `↓n`=layer-tap to layer n

---

#### Layer 0 — BASE (QWERTY)

```
┌─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┐
│  Q  │  W  │  E  │  R  │  T  │       │  Y  │  U  │  I  │  O  │  P  │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│ A/⇧ │ S/⌃ │ D/⌥ │ F/⌘ │ G/✦ │       │ H/✦ │ J/⌘ │ K/⌥ │ L/⌃ │ ;/⇧ │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│  Z  │  X  │  C  │  V  │  B  │       │  N  │  M  │  ,  │  .  │  /  │
└─────┼─────┼─────┼─────┼─────┘       └─────┼─────┼─────┼─────┼─────┘
      │LCLK │CAPS │Tab↓1│Spc↓3│       │Ent↓5│Bsp↓4│ CW  │RCLK │
      └─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┘
```

> LCLK / RCLK = mouse buttons · CW = Caps Word · home-row: hold for modifier, tap for key

---

#### Layer 1 — SYM (Symbols)

```
┌─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┐
│  !  │  @  │  #  │  $  │  %  │       │  ^  │  &  │  *  │  (  │  )  │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│ ./⇧ │ ,/⌃ │ "/⌥ │ '/⌘ │  |  │       │  -  │ =/⌘ │ `/⌥ │ {/⌃ │ }/⇧ │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│  ?  │  /  │  <  │  >  │  \  │       │  _  │  +  │  ~  │  [  │  ]  │
└─────┼─────┼─────┼─────┼─────┘       └─────┼─────┼─────┼─────┼─────┘
      │LCLK │ --- │ --- │ --- │       │ Ent │ Del │ --- │RCLK │
      └─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┘
```

---

#### Layer 2 — EMPTY *(intentionally blank — firmware bug workaround)*

---

#### Layer 3 — NAV (Navigation)

```
┌─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┐
│CAPS │     │     │     │     │       │HOME │PgDn │PgUp │ END │ Del │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│  ⇧  │  ⌃  │  ⌥  │  ⌘  │     │       │  ←  │  ↓  │  ↑  │  →  │Bspc │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│     │     │ ⌘C  │ ⌘V  │     │       │     │     │     │     │     │
└─────┼─────┼─────┼─────┼─────┘       └─────┼─────┼─────┼─────┼─────┘
      │LCLK │ --- │ Tab │ --- │       │ Ent │ Del │ --- │RCLK │
      └─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┘
```

---

#### Layer 4 — NUM (Numbers & Function keys)

```
┌─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┐
│ F1  │ F2  │ F3  │ F4  │ F5  │       │ F6  │ F7  │ F8  │ F9  │F10  │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│ 1/⇧ │ 2/⌃ │ 3/⌥ │ 4/⌘ │  5  │       │  6  │ 7/⌘ │ 8/⌥ │ 9/⌃ │ 0/⇧ │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│F11  │     │     │     │     │       │     │     │     │     │F12  │
└─────┼─────┼─────┼─────┼─────┘       └─────┼─────┼─────┼─────┼─────┘
      │LCLK │ --- │ Tab │ Spc │       │ Ent │ --- │ --- │RCLK │
      └─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┘
```

---

#### Layer 5 — MEDIA (Bluetooth & System)

```
┌─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┐
│ BT0 │     │ |<< │ >>| │ >|| │       │ BT3 │     │     │UNSTK│ RST │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│ BT1 │     │VOL- │VOL+ │MUTE │       │ BT4 │     │     │     │BOOT │
├─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┤
│ BT2 │     │BRI- │BRI+ │     │       │ BT5 │     │     │     │BTCLR│
└─────┼─────┼─────┼─────┼─────┘       └─────┼─────┼─────┼─────┼─────┘
      │LCLK │ --- │ --- │ --- │       │ --- │ --- │ --- │RCLK │
      └─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┘
```

> BT0–BT5 = Bluetooth profile · UNSTK = unstick modifiers · BTCLR = clear BT bond

---

#### Combos

| Keys | Layer | Output |
|------|-------|--------|
| S + D | BASE | ESC |
| K + L | BASE | ESC |

## Building

Firmware is built automatically via GitHub Actions on every push.
Download the `.uf2` from the Actions tab → latest run → Artifacts.

### Local build (optional)

```bash
# one-time setup
west init -l config
west update

# build
west build -s zmk/app -b nice_nano_v2 -- -DSHIELD=kb38 -DZMK_CONFIG="$(pwd)/config"
```

## Flashing

1. Double-tap RST to enter bootloader (drive named `NICENANO` appears)
2. Drag & drop `zmk.uf2` onto the drive

## SuperMini NRF52840 notes

Works as `nice_nano_v2`. Known differences:

- **LED colors swapped**: blue = BT status, red = charging (cosmetic only)
- **Battery reporting**: voltage divider on P0.24 is unpopulated by default →
  battery % will not work. Options:
  - Populate the voltage divider resistors (see SuperMini schematic)
  - Disable: add `CONFIG_ZMK_BATTERY_REPORTING=n` to `config/kb38.conf`
- **Higher sleep current** (~700 µA vs ~20 µA on genuine nice!nano)

## Known issues

- Layer index 2 is intentionally empty — ZMK firmware bug: layer 2 + key position 2
  causes BLE disconnect. Do not assign bindings to layer 2.

## Cirque I2C mode

TM035035-2024-003 is pre-configured for I2C (ADDR pin handled).
I2C address: 0x2A. Driver uses polling mode (no DR pin required).