#!/bin/bash

# Clean the .esphome directory
rm -rf .esphome

# Run the ESPHome configuration
esphome run local.yaml