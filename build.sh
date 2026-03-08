#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────────────────
# KB38 — local ZMK build script
#
# Usage:
#   ./build.sh            — build firmware (incremental)
#   ./build.sh --clean    — pristine rebuild from scratch
#   ./build.sh --flash    — build + copy .uf2 to mounted bootloader drive
#   ./build.sh --help
#
# Prerequisites (macOS / Linux):
#   brew install cmake ninja python3 ccache   # macOS
#   pip3 install west                         # both
#   ARM toolchain: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
#   (or via Homebrew: brew install --cask gcc-arm-embedded)
#
# First-time setup (run once in this directory):
#   west init -l config
#   west update
# ─────────────────────────────────────────────────────────────────────────────
set -euo pipefail

BOARD="nice_nano_v2"
SHIELD="kb38"
BUILD_DIR="build"
ZMK_CONFIG="$(pwd)/config"
UF2_SRC="$BUILD_DIR/zephyr/zmk.uf2"

# ── macOS: bootloader drive name (double-tap RST to enter DFU) ───────────────
BOOTLOADER_VOLUME="/Volumes/NICENANO"

# ─────────────────────────────────────────────────────────────────────────────
usage() {
    echo "Usage: $0 [--clean] [--flash] [--help]"
    echo ""
    echo "  (no args)   incremental build"
    echo "  --clean     pristine rebuild (deletes build/ dir)"
    echo "  --flash     build + copy .uf2 to $BOOTLOADER_VOLUME"
    echo "  --help      this message"
}

CLEAN=0
FLASH=0

for arg in "$@"; do
    case "$arg" in
        --clean) CLEAN=1 ;;
        --flash) FLASH=1 ;;
        --help)  usage; exit 0 ;;
        *) echo "Unknown argument: $arg"; usage; exit 1 ;;
    esac
done

# ── sanity checks ─────────────────────────────────────────────────────────────
if ! command -v west &>/dev/null; then
    echo "ERROR: 'west' not found. Install with: pip3 install west"
    exit 1
fi

if [[ ! -d ".west" ]]; then
    echo "ERROR: West workspace not initialised."
    echo "Run once: west init -l config && west update"
    exit 1
fi

# ── clean ─────────────────────────────────────────────────────────────────────
if [[ $CLEAN -eq 1 ]]; then
    echo "── Pristine build: removing $BUILD_DIR/ ──"
    rm -rf "$BUILD_DIR"
fi

# ── build ─────────────────────────────────────────────────────────────────────
echo "── Building $SHIELD on $BOARD ──"
west build \
    --build-dir "$BUILD_DIR" \
    --source-dir zmk/app \
    --board "$BOARD" \
    -- \
    -DSHIELD="$SHIELD" \
    -DZMK_CONFIG="$ZMK_CONFIG"

echo ""
echo "── Build complete ──"
echo "Firmware: $UF2_SRC"

# ── flash ─────────────────────────────────────────────────────────────────────
if [[ $FLASH -eq 1 ]]; then
    echo ""
    echo "── Flash: waiting for $BOOTLOADER_VOLUME ──"
    echo "Double-tap RST on the controller now…"

    # Wait up to 30 s for the drive to appear
    TIMEOUT=30
    ELAPSED=0
    until [[ -d "$BOOTLOADER_VOLUME" ]]; do
        sleep 1
        ELAPSED=$((ELAPSED + 1))
        if [[ $ELAPSED -ge $TIMEOUT ]]; then
            echo "ERROR: $BOOTLOADER_VOLUME did not appear within ${TIMEOUT}s."
            echo "Check that the controller is in bootloader mode."
            exit 1
        fi
    done

    echo "Drive found — copying .uf2…"
    cp "$UF2_SRC" "$BOOTLOADER_VOLUME/"
    echo "Done. Controller will reboot automatically."
fi
