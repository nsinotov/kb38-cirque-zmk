# KB38 — Wiring Guide

Controller: **nice!nano v2** (SuperMini NRF52840 is pin-compatible)

## Pinout

```
                  USB-C
         ┌──────────────────┐
    GND  │  L1         R1   │  GND
    GND  │  L2         R2   │  RAW  <- LiPo + terminal
    VCC  │  L3         R3   │  3.3V out
     D1  │  L4  P0.06  R4   │  RST
     D0  │  L5  P0.08  R5   │  D21  P0.31
     D2  │  L6  P0.17  R6   │  D20  P0.29
     D3  │  L7  P0.20  R7   │  D19  P0.02  <- I2C SCL
     D4  │  L8  P0.22  R8   │  D18  P1.15  <- I2C SDA
     D5  │  L9  P0.24  R9   │  D15  P1.13
     D6  │  L10 P1.00  R10  │  D14  P1.11
     D7  │  L11 P0.11  R11  │  D16  P0.10
     D8  │  L12 P1.04  R12  │  D10  P0.09
     D9  │  L13 P1.06       │
         └──────────────────┘

LED (P0.15) is internal, not exposed on any pad.
```

## Matrix

Diode direction: **col-to-row** (cathode stripe toward row wire).

### Rows

| GPIO | Pad | Row |
|------|-----|-----|
| P0.22 | D4 | R0 — Q W E R T / Y U I O P |
| P0.24 | D5 | R1 — A S D F G / H J K L ; |
| P1.00 | D6 | R2 — Z X C V B / N M , . / |
| P0.11 | D7 | R3 — thumb row |

### Columns

| GPIO | Pad | Keys |
|------|-----|------|
| P0.08 | D0  | C0 — Q/A/Z |
| P0.06 | D1  | C1 — W/S/X/LCLK |
| P0.20 | D3  | C2 — E/D/C/CW |
| P1.04 | D8  | C3 — R/F/V/Tab |
| P1.06 | D9  | C4 — T/G/B/Space |
| P0.09 | D10 | C5 — Y/H/N/Enter |
| P1.11 | D14 | C6 — U/J/M/Bspc |
| P1.13 | D15 | C7 — I/K/,/CW |
| P0.10 | D16 | C8 — O/L/./RCLK |
| P0.31 | D21 | C9 — P/;// |

## Cirque Trackpad (TM035035 — I2C)

I2C address: **0x2A**

| Signal | GPIO | Pad | Note |
|--------|------|-----|------|
| SDA | P1.15 | D18 | 4.7k pull-up to 3.3V |
| SCL | P0.02 | D19 | 4.7k pull-up to 3.3V |
| DR  | P0.17 | D2  | Data-ready interrupt (active HIGH) |
| VDD | —     | VCC | 3.3V regulated (not RAW) |
| GND | —     | GND | |

Uses I2C1 (TWIM1) with pin select via `pinctrl`. I2C0 is disabled to avoid
conflicts with matrix pins D2 (P0.17) and D3 (P0.20). SPI1 is disabled
because SPIM1 and TWIM1 share the same hardware peripheral on nRF52840.

External 4.7k pull-ups to 3.3V are required on SDA and SCL. The Cirque
trackpad has no built-in pull-ups. Internal pull-ups are also enabled in
pinctrl as a fallback.

## Battery

| LiPo wire | Pad |
|-----------|-----|
| + | RAW |
| - | GND |

Recommended cell: 301230 (~100 mAh, fits under socketed nice!nano).
Charge rate: ~100 mA. Do not use batteries < 100 mAh without adjusting
charge resistor.

Battery reporting requires the voltage divider on P0.24 to be populated.
Currently disabled in `kb38.conf`.

## GPIO Summary

```
Matrix rows:  P0.22  P0.24  P1.00  P0.11
               R0     R1     R2     R3

Matrix cols:  P0.08  P0.06  P0.20  P1.04  P1.06
               C0     C1     C2     C3     C4

              P0.09  P1.11  P1.13  P0.10  P0.31
               C5     C6     C7     C8     C9

Cirque I2C:   SDA = P1.15 (D18)   SCL = P0.02 (D19)
Cirque DR:    P0.17 (D2)

LED:          P0.15 (internal)
Battery:      RAW(+)  GND(-)
```
