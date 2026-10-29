#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "fan_buttons.h"
#include "fan_sensors.h"
#include "fan_mqtt.h"

static unsigned long lastSensorPublishMs = 0;

// ---------- Wi-Fi ----------
static void connect_wifi() {
    Serial.printf("[WiFi] Menghubungkan ke %s", WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(WIFI_HOSTNAME);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
        delay(400);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\n[WiFi] Terhubung. IP: %s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println("\n[WiFi] Gagal. Akan dicoba di loop.");
    }
}

// ---------- Callback command dari HA ----------
static void on_command(const char* topic, const char* payload) {
    Serial.printf("[CMD] %s → %s\n", topic, payload);
    // Contoh feedback: kedipkan LED saat perintah masuk
    digitalWrite(PIN_STATUS_LED, HIGH);
    delay(80);
    digitalWrite(PIN_STATUS_LED, LOW);
}

// ---------- Setup ----------
void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println();
    Serial.println("🪭 FANORA — Panel Kipas Digital");
    Serial.println("================================");
    Serial.printf("Firmware : %s\n", FANORA_FW_VERSION);
    Serial.printf("Device ID: %s\n\n", FANORA_DEVICE_ID);

    pinMode(PIN_STATUS_LED, OUTPUT);
    digitalWrite(PIN_STATUS_LED, LOW);

    buttons_init();
    sensors_init();
    mqtt_init();
    mqtt_set_command_callback(on_command);

    connect_wifi();
}

// ---------- Loop ----------
void loop() {
    if (WiFi.status() != WL_CONNECTED) connect_wifi();

    buttons_update();
    sensors_update();
    mqtt_update();

    // --- Publish event untuk setiap tombol yang ditekan ---
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        if (buttons_was_toggled(i)) {
            mqtt_publish_button(i, true);
            mqtt_publish_event(i, "single_press");
            Serial.printf("[BTN] Tombol %u ditekan (total %lu)\n",
                          i + 1, buttons_get_press_count(i));
        }
    }

    // --- Publish sensor periodik ---
    unsigned long now = millis();
    if (now - lastSensorPublishMs >= SENSOR_INTERVAL_MS) {
        lastSensorPublishMs = now;
        if (sensors_has_new_reading()) {
            mqtt_publish_temperature(sensors_get_temperature());
            mqtt_publish_humidity(sensors_get_humidity());
        }
    }

    // --- Publish gerak saat berubah ---
    if (sensors_motion_changed()) {
        mqtt_publish_motion(sensors_get_motion());
    }
}
