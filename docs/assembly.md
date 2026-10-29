# 🔧 Panduan Perakitan Fanora

## 1. Persiapan (15 menit)

1. Solder **8 tombol** pada PCB sesuai pinout (`hardware/pinout.md`).
2. Pasang **pull-up internal** ESP32 (default `INPUT_PULLUP`) — tidak perlu resistor eksternal untuk tombol.
3. Solder **DHT22** + resistor **10 kΩ** (DATA→3V3).
4. Solder **PIR HC-SR501** (VCC→3V3, GND→GND, OUT→GPIO 22).
5. Pasang **USB-C breakout** untuk catu daya.

## 2. Uji Rangkaian (10 menit)

```bash
pio run -t upload
pio device monitor -b 115200
