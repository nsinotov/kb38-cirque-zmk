# KB38 — Wiring Guide

Controller: **nice!nano v2** (or compatible SuperMini NRF52840)

---

## nice!nano v2 Pinout

```
                  USB-C
         ┌──────────────────┐
    GND  │  L1         R1   │  GND
    GND  │  L2         R2   │  VCC (3.3V out)
    VCC  │  L3         R3   │  RAW  ← LiPo + terminal
     D1  │  L4  P0.06  R4   │  RST
     D0  │  L5  P0.08  R5   │  D21  P0.31
   ★ D2  │  L6  P0.15  R6   │  D20  P0.30  (bat sense, do not use)
     D3  │  L7  P0.17  R7   │  D19  P0.29  ← I2C SCL (Cirque)
     D4  │  L8  P0.20  R8   │  D18  P0.28  ← I2C SDA (Cirque)
     D5  │  L9  P0.22  R9   │  D16  P1.15
     D6  │  L10 P0.24  R10  │  D15  P1.13
     D7  │  L11 P1.00  R11  │  D14  P1.11
     D8  │  L12 P0.11  R12  │  D10  P1.06
     D9  │  L13 P1.04       │
         └──────────────────┘

★ D2 (P0.15) = onboard blue LED — do not use as GPIO
```

> **SuperMini note:** LED colors are swapped vs nice!nano — blue = BT status, red = charging.  
> The voltage divider for battery reporting (P0.24) is unpopulated. If battery % shows 0,
> add `CONFIG_ZMK_BATTERY_REPORTING=n` in `kb38.conf`, or populate the divider circuit.

---

## Matrix Connections

### Rows (controller output → diode row wire)

| Row | D-pin | nRF GPIO | Physical keys        |
|-----|-------|----------|----------------------|
| R0  | D4    | P0.20    | Top row (Q…P)        |
| R1  | D5    | P0.22    | Home row (A…;)       |
| R2  | D6    | P0.24    | Bottom row (Z…/)     |
| R3  | D7    | P1.00    | Thumb row            |

### Columns (key matrix column → controller input)

| Col | D-pin | nRF GPIO | Physical keys (left→right) |
|-----|-------|----------|---------------------------|
| C0  | D0    | P0.08    | Q / A / Z / —             |
| C1  | D1    | P0.06    | W / S / X / LCLK          |
| C2  | D3    | P0.17    | E / D / C / CapsWord      |
| C3  | D8    | P0.11    | R / F / V / Tab↓SYM       |
| C4  | D9    | P1.04    | T / G / B / Space↓NAV     |
| C5  | D10   | P1.06    | Y / H / N / Enter↓MEDIA   |
| C6  | D14   | P1.11    | U / J / M / Bspc↓NUM      |
| C7  | D15   | P1.13    | I / K / , / CapsWord      |
| C8  | D16   | P1.15    | O / L / . / RCLK          |
| C9  | D21   | P0.31    | P / ; / / / —             |

---

## Diodes

Direction: **col-to-row** (cathode = stripe toward the row wire).

```
   Col wire ──[>|── Row wire
                ^
            cathode stripe
```

Recommended: 1N4148 SOD-123 (SMD) or through-hole.

---

## Cirque Trackpad (TM035035 — I2C mode)

| Cirque pin | Controller | Note                         |
|------------|------------|------------------------------|
| VCC        | VCC (3.3V) | 3.3V only — never 5V         |
| GND        | GND        |                              |
| SDA        | D18 (R8)   | P0.28 — pull-up via 4.7kΩ¹   |
| SCL        | D19 (R7)   | P0.29 — pull-up via 4.7kΩ¹   |
| DR         | —          | Data-ready pin; not used (polling mode) |

¹ The overlay enables internal pull-ups (`bias-pull-up`).  
  External 4.7kΩ pull-ups to 3.3V are recommended for cable lengths > 10 cm.

> I2C address: **0x2A** (confirmed for TM035035 in I2C mode).

---

## Battery

| LiPo wire | Controller |
|-----------|------------|
| +         | RAW (R3)   |
| −         | GND        |

Recommended cell: 301230 (~100 mAh, fits under socketed nice!nano).  
Charge rate: ~100 mA. Do not use batteries < 100 mAh without adjusting charge resistor.

---

## LED Indicator

Uses the onboard LED only (no external LED required — all GPIO are occupied by matrix + I2C).

| Feature                 | Behavior                       |
|-------------------------|--------------------------------|
| BLE advertising (pair)  | Fast blink                     |
| BLE connected           | Slow blink / solid             |
| Battery low (< 20%)     | Short pulse on wake            |

Controlled by `CONFIG_ZMK_BLE_STATUS_LED=y`.

---

## USB-C Access

nice!nano has USB-C on top of the board.  
Options for routing it to the case exterior:

1. **Direct placement** — position the controller flush with a cutout in the case wall.
2. **Flat ribbon extension** — use a 5–10 cm USB-C flat FPC extension cable

---

## Summary Diagram

```
               ┌──────────────────────────────────────────────────────┐
               │                   nice!nano v2                       │
               │                                                      │
    COL wires  │  D0  D1  D3  D8  D9   D10 D14 D15 D16 D21            │
  ─────────────┤  C0  C1  C2  C3  C4   C5  C6  C7  C8  C9             │
               │                                                      │
    ROW wires  │            D4  D5  D6  D7                            │
  ─────────────┤            R0  R1  R2  R3                            │
               │                                                      │
    Cirque     │                       D18=SDA  D19=SCL               │
  ─────────────┤                       ←────────────────              │
               │                                                      │
    Battery    │  RAW(+)  GND(−)                                      │
  ─────────────┤                                                      │
               └──────────────────────────────────────────────────────┘
```
