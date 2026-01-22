#!/bin/bash
# Build firmware and copy to local support site for testing

set -e

VERSION="1.0.10"
DEST="/Users/foz/src/support-soliddifference/content/downloads"

echo "Building Siebenuhr ESPHome firmware v$VERSION"
echo "Destination: $DEST"
echo ""

# Build Mini
echo "Building Mini firmware..."
uv run esphome compile siebenuhr_mini.yaml

# Build Regular
echo "Building Regular firmware..."
uv run esphome compile siebenuhr_regular.yaml

# Copy factory images to support site
echo ""
echo "Copying firmware to support site..."

cp .esphome/build/siebenuhr-mini/.pioenvs/siebenuhr-mini/firmware.factory.bin \
   "$DEST/firmware-mini-esphome-$VERSION.bin"

cp .esphome/build/siebenuhr-regular/.pioenvs/siebenuhr-regular/firmware.factory.bin \
   "$DEST/firmware-regular-esphome-$VERSION.bin"

echo ""
echo "Done! Files copied:"
ls -la "$DEST"/firmware-*-esphome-*.bin
