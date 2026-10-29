#ifndef FANORA_CONFIG_H
#define FANORA_CONFIG_H

// ============================================================
//  🪭 FANORA — Konfigurasi Perangkat
//  Sesuaikan nilai di bawah ini dengan lingkungan Anda.
// ============================================================

// ---- Identitas Perangkat ----
#define FANORA_DEVICE_ID    "fanora_01"
#define FANORA_DEVICE_NAME  "Fanora — Panel Kipas"
#ifndef FANORA_FW_VERSION
#define FANORA_FW_VERSION   "1.0.0"
#endif

// ---- Wi-Fi ----
#define WIFI_SSID           "GANTI_DENGAN_SSID_ANDA"
#define WIFI_PASSWORD       "GANTI_DENGAN_PASSWORD"
#define WIFI_HOSTNAME       "fanora-01"

// ---- MQTT (Home Assistant) ----
#define MQTT_HOST           "192.168.1.10"   // IP Home Assistant / Broker
#define MQTT_PORT           1883
#define MQTT_USER           "fanora"
#define MQTT_PASSWORD       "rahasia"
#define MQTT_BASE_TOPIC     "fanora/" FANORA_DEVICE_ID

// ---- Pin Mapping ----
// 8 tombol tersusun melengkung — tulang kipas
#define PIN_BTN_1           4
#define PIN_BTN_2           5
#define PIN_BTN_3           13
#define PIN_BTN_4           16
#define PIN_BTN_5           17
#define PIN_BTN_6           18
#define PIN_BTN_7           19
#define PIN_BTN_8           21

#define PIN_PIR             22   // Sensor gerak (di balik "giok")
#define PIN_DHT             23   // DHT22 (suhu + kelembaban)
#define PIN_STATUS_LED      2    // LED bawaan ESP32

// ---- Parameter ----
#define BUTTON_COUNT        8
#define DEBOUNCE_MS         50
#define SENSOR_INTERVAL_MS  5000
#define MQTT_RECONNECT_MS   5000
#define DHT_TYPE            DHT22

#endif // FANORA_CONFIG_H
