/home/cpasjuste/.platformio/penv/bin/pio run -e pico-probe
Processing pico-probe (platform: https://github.com/maxgerhardt/platform-raspberrypi.git; framework: arduino; board: pico)
--------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/raspberrypi/pico.html
PLATFORM: Raspberry Pi RP2040 (1.10.0+sha.79ebb7d) > Raspberry Pi Pico
HARDWARE: RP2040 133MHz, 264KB RAM, 2MB Flash
DEBUG: Current (cmsis-dap) External (blackmagic, cmsis-dap, jlink, picoprobe, raspberrypi-swd)
PACKAGES:
- framework-arduinopico @ 1.30601.0+sha.c3a3526
- tool-rp2040tools @ 1.0.2
- toolchain-rp2040-earlephilhower @ 5.120300.230911 (12.3.0)
  Flash size: 2.00MB
  Sketch size: 1.00MB
  Filesystem size: 1.00MB
  Maximium Sketch size: 1044480 EEPROM start: 0x101ff000 Filesystem start: 0x100ff000 Filesystem end: 0x101ff000
  LDF: Library Dependency Finder -> https://bit.ly/configure-pio-ldf
  LDF Modes: Finder ~ chain, Compatibility ~ soft
  Found 59 compatible libraries
  Scanning dependencies...
  Dependency Graph
  |-- Wire @ 1.0
  |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |-- Adafruit SSD1306 @ 2.5.9
  |   |-- Adafruit GFX Library @ 1.11.9
  |   |   |-- Adafruit BusIO @ 1.14.5
  |   |   |   |-- Wire @ 1.0
  |   |   |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |   |   |-- SPI @ 1.0
  |   |   |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |   |-- Wire @ 1.0
  |   |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |   |-- SPI @ 1.0
  |   |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |-- Adafruit BusIO @ 1.14.5
  |   |   |-- Wire @ 1.0
  |   |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |   |-- SPI @ 1.0
  |   |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |-- Wire @ 1.0
  |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |-- SPI @ 1.0
  |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |-- Adafruit BusIO @ 1.14.5
  |   |-- Wire @ 1.0
  |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |-- SPI @ 1.0
  |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |-- Adafruit GFX Library @ 1.11.9
  |   |-- Adafruit BusIO @ 1.14.5
  |   |   |-- Wire @ 1.0
  |   |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |   |-- SPI @ 1.0
  |   |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |-- Wire @ 1.0
  |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |-- SPI @ 1.0
  |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |-- ArduinoJson @ 6.21.4
  |-- Adafruit SPIFlash @ 4.1.1
  |   |-- SPI @ 1.0
  |   |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |-- SdFat - Adafruit Fork @ 2.2.1
  |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |-- SPI @ 1.0
  |   |-- Adafruit TinyUSB Library @ 2.0.1
  |   |   |-- SdFat - Adafruit Fork @ 2.2.1
  |-- SdFat - Adafruit Fork @ 2.2.1
  |-- Adafruit TinyUSB Library @ 2.0.1
  |   |-- SdFat - Adafruit Fork @ 2.2.1
  Building in debug mode
  Retrieving maximum program size .pio/build/pico-probe/firmware.elf
  Flash size: 2.00MB
  Sketch size: 1.00MB
  Filesystem size: 1.00MB
  Maximium Sketch size: 1044480 EEPROM start: 0x101ff000 Filesystem start: 0x100ff000 Filesystem end: 0x101ff000
  Checking size .pio/build/pico-probe/firmware.elf
  Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
  RAM:   [=         ]   7.6% (used 19944 bytes from 262144 bytes)
  Flash: [==        ]  17.0% (used 177968 bytes from 1044480 bytes)
  Building .pio/build/pico-probe/firmware.bin.signed
  ========================= [SUCCESS] Took 2.33 seconds =========================

Environment    Status    Duration
-------------  --------  ------------
pico-probe     SUCCESS   00:00:02.335
========================= 1 succeeded in 00:00:02.335 =========================
