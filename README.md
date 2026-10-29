<div align="center">

# 🪭 Fanora

### 扇形 · Panel Kipas Digital

**Kearifan Tiongkok Kuno × Kecepatan Shenzhen Modern**

[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-blue?logo=platformio)](https://platformio.org/)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-MQTT-41BDF5?logo=homeassistant)](https://www.home-assistant.io/)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)
[![Made in Shenzhen](https://img.shields.io/badge/Spirit-Shenzhen-red)]()

</div>

---

## 缘 · Filosofi

> *"扇开一半，可见天地。开至全展，乃纳万物。"*
>
> *"Kipas dibuka setengah, terlihat langit-bumi. Dibuka penuh, menampung segala."*
> — Pepatah klasik tentang keseimbangan

**Fanora** (dari 扇 *shàn* = kipas, dan *Aurora* = cahaya) adalah panel kontrol rumah yang mengambil bentuk **kipas lipat Tiongkok kuno** — delapan tombol tersusun melengkung seperti tulang kipas, dengan "batu giok" di tengahnya yang menyembunyikan sensor gerak dan suhu.

Setiap tombol mengontrol satu perangkat di rumah Anda via **Home Assistant** lokal. Tidak ada cloud. Tidak ada ketergantungan pihak ketiga. Hanya Anda, rumah, dan keheningan.

---

## 技 · Spesifikasi Teknis

| Komponen | Spesifikasi | Filosofi |
|---|---|---|
| **Otak** | ESP32-WROOM-32 (dual-core, 240 MHz) | 心 · Jantung |
| **Input** | 8× tombol mekanik (tulang kipas) | 骨 · Tulang |
| **Sensor** | PIR + DHT22 (tersembunyi di balik "giok") | 眼 · Mata |
| **Konektivitas** | Wi-Fi 2.4 GHz + MQTT | 络 · Jaringan |
| **Protokol** | Home Assistant MQTT Discovery | 语 · Bahasa |
| **Catu Daya** | 5 V USB-C (3 W) | 气 · Energi |
| **Firmware** | PlatformIO / Arduino Framework | 律 · Hukum |

---

## 图 · Diagram Blok

```
        ┌──────────────────────────────────────┐
        │            🪭 FANORA                  │
        │                                      │
        │   [Btn 1]  [Btn 2]  [Btn 3]  [Btn 4] │
        │      \      |        |       /       │
        │       \     └── 🟢 ──┘      /        │
        │        \    ESP32-WROOM    /         │
        │         ─── (Giok Hetian) ───        │
        │        /   PIR  DHT22    \           │
        │       /                   \          │
        │   [Btn 8]  [Btn 7]  [Btn 6]  [Btn 5] │
        │                                      │
        └──────────────┬───────────────────────┘
                       │ Wi-Fi 2.4 GHz
                       ▼
              ┌─────────────────┐
              │  MQTT Broker    │
              │  (Mosquitto)    │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │ Home Assistant  │
              │  (Local-First)  │
              └─────────────────┘
```

---

## 装 · Cara Pasang

### 1. Clone & Setup Firmware

```bash
git clone https://github.com/<anda>/fanora.git
cd fanora
cp src/config.h.example src/config.h   # lalu edit
```

Edit `src/config.h`:
```c
#define WIFI_SSID     "NamaWifiAnda"
#define WIFI_PASSWORD "PasswordWifi"
#define MQTT_HOST     "192.168.1.10"     // IP Home Assistant
#define MQTT_USER     "fanora"
#define MQTT_PASSWORD "rahasia"
```

### 2. Build & Flash

```bash
pio run -t upload
pio device monitor
```

### 3. Integrasi Home Assistant

Tambahkan blok MQTT ke `configuration.yaml`:

```yaml
mqtt:
  broker: 192.168.1.10
  username: fanora
  password: rahasia
```

Restart HA — perangkat **Fanora** akan muncul otomatis di **Settings → Devices** (MQTT Discovery).

---

## 用 · Penggunaan

Setiap tombol fisik akan memicu event di Home Assistant:

```yaml
# Contoh automation: tombol 1 → hidupkan lampu ruang tamu
automation:
  - alias: "Fanora · Tombol 1 → Lampu Tamu"
    trigger:
      - platform: state
        entity_id: event.fanora_tombol_1
    condition:
      - condition: template
        value_template: "{{ trigger.to_state.attributes.event_type == 'single_press' }}"
    action:
      - service: light.toggle
        target:
          entity_id: light.ruang_tamu
```

Lihat `homeassistant/automations_example.yaml` untuk 8 contoh lengkap.

---

## 料 · Daftar Komponen (BOM)

Lihat [`hardware/BOM.md`](hardware/BOM.md) untuk daftar lengkap dengan harga indikatif dari **Huaqiangbei, Shenzhen**.

| Item | Jumlah | Perkiraan Harga |
|---|---|---|
| ESP32-WROOM-32 DevKit | 1 | Rp 55.000 |
| Tombol mekanik 12 mm (tactile) | 8 | Rp 4.000 |
| Sensor PIR HC-SR501 | 1 | Rp 15.000 |
| DHT22 (AM2302) | 1 | Rp 25.000 |
| Resistor 10 kΩ (pull-up DHT) | 1 | Rp 500 |
| Batu giok sintetis (centerpiece) | 1 | Rp 30.000 |
| Casing (3D print / CNC aluminium) | 1 | Rp 80.000 |
| Kabel, PCB, konektor USB-C | — | Rp 20.000 |
| **Total** | | **≈ Rp 230.000** |

---

## 展 · Galeri Konsep

| Komponen | Material | Tekstur |
|---|---|---|
| Body | Aluminium 6061 anodized matte | Hangat, seperti bambu |
| Tulang kipas | Akrilik susu (3 mm) | Lembut, tembus cahaya |
| Centerpiece | Batu giok sintetis (Hetian) | Dingin, berkilau |
| Aksen tombol | Laser engrave motif awan (祥云) | Tradisional |

---

## 律 · Lisensi

MIT License — bebas digunakan, dimodifikasi, dan dibagikan.
Dibuat dengan 🪭 dan ☕ di antara **budaya leluhur** dan **pasar elektronik terbesar dunia**.

---

<div align="center">

**扇开 · 万物互联**
*"Kipas dibuka, segala terhubung."*

</div>
