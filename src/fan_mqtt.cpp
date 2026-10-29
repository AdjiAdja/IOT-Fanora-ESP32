#include "fan_mqtt.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

static WiFiClient       espClient;
static PubSubClient     client(espClient);
static unsigned long    lastReconnectMs = 0;
static CommandCallback  cmdCallback     = nullptr;

// ---------- Helpers ----------
static String topicStr(const char* suffix) {
    String s = MQTT_BASE_TOPIC;
    s += "/";
    s += suffix;
    return s;
}

static String topicStr(const String& suffix) {
    return topicStr(suffix.c_str());
}

static void publish_raw(const String& topic, const String& payload, bool retain = false) {
    if (client.connected()) {
        client.publish(topic.c_str(), payload.c_str(), retain);
    }
}

// ---------- Callback internal ----------
static void internalCallback(char* topic, byte* payload, unsigned int len) {
    static char buf[256];
    if (len >= sizeof(buf)) len = sizeof(buf) - 1;
    memcpy(buf, payload, len);
    buf[len] = '\0';
    if (cmdCallback) cmdCallback(topic, buf);
}

// ---------- Koneksi ----------
static void connect() {
    String clientId  = String(FANORA_DEVICE_ID) + "-" + String(random(0xffff), HEX);
    String lwtTopic  = topicStr("status");

    bool ok = client.connect(
        clientId.c_str(),
        MQTT_USER, MQTT_PASSWORD,
        lwtTopic.c_str(), 1, true, "offline"
    );

    if (ok) {
        Serial.println("[MQTT] Terhubung.");
        mqtt_publish_status("online");
        client.subscribe(topicStr("cmd").c_str());
        mqtt_send_discovery();
    } else {
        Serial.printf("[MQTT] Gagal, rc=%d\n", client.state());
    }
}

void mqtt_init() {
    client.setServer(MQTT_HOST, MQTT_PORT);
    client.setCallback(internalCallback);
    client.setBufferSize(1024);
}

void mqtt_update() {
    if (!client.connected()) {
        unsigned long now = millis();
        if (now - lastReconnectMs >= MQTT_RECONNECT_MS) {
            lastReconnectMs = now;
            connect();
        }
    } else {
        client.loop();
    }
}

bool mqtt_is_connected() { return client.connected(); }

// ---------- Publish ----------
void mqtt_publish_button(uint8_t index, bool pressed) {
    publish_raw(topicStr("button/" + String(index + 1)), pressed ? "ON" : "OFF");
}

void mqtt_publish_event(uint8_t index, const char* eventType) {
    publish_raw(topicStr("event/" + String(index + 1)), eventType);
}

void mqtt_publish_temperature(float t) {
    publish_raw(topicStr("sensor/temperature"), String(t, 2));
}

void mqtt_publish_humidity(float h) {
    publish_raw(topicStr("sensor/humidity"), String(h, 2));
}

void mqtt_publish_motion(bool motion) {
    publish_raw(topicStr("sensor/motion"), motion ? "ON" : "OFF");
}

void mqtt_publish_status(const char* status) {
    publish_raw(topicStr("status"), status, true);
}

void mqtt_set_command_callback(CommandCallback cb) {
    cmdCallback = cb;
}

// ---------- Home Assistant Discovery ----------
static void pub_discovery(const String& component, const String& objectId, JsonDocument& doc) {
    String topic = String("homeassistant/") + component + "/"
                 + FANORA_DEVICE_ID + "/" + objectId + "/config";
    String payload;
    serializeJson(doc, payload);
    publish_raw(topic, payload, true);
}

static void add_device_block(JsonDocument& doc) {
    JsonObject dev = doc["device"].to<JsonObject>();
    JsonArray ids  = dev["ids"].to<JsonArray>();
    ids.add(FANORA_DEVICE_ID);
    dev["name"] = FANORA_DEVICE_NAME;
    dev["mf"]   = "Shenzhen Maker Guild";
    dev["mdl"]  = "Fanora v1 (Kipas Lipat)";
    dev["sw"]   = FANORA_FW_VERSION;
}

static void add_availability(JsonDocument& doc) {
    doc["avty_t"]       = topicStr("status");
    doc["pl_avail"]     = "online";
    doc["pl_not_avail"] = "offline";
}

void mqtt_send_discovery() {
    // --- 8 tombol (event entity) ---
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        JsonDocument doc;
        doc["name"]    = "Tombol " + String(i + 1);
        doc["uniq_id"] = String(FANORA_DEVICE_ID) + "_btn" + String(i + 1);
        doc["stat_t"]  = topicStr("event/" + String(i + 1));

        JsonArray types = doc["event_types"].to<JsonArray>();
        types.add("single_press");
        types.add("double_press");
        types.add("long_press");

        add_device_block(doc);
        add_availability(doc);
        pub_discovery("event", "button_" + String(i + 1), doc);
    }

    // --- Suhu ---
    {
        JsonDocument doc;
        doc["name"]        = "Suhu";
        doc["uniq_id"]     = String(FANORA_DEVICE_ID) + "_temperature";
        doc["stat_t"]      = topicStr("sensor/temperature");
        doc["unit_of_meas"]= "°C";
        doc["dev_cla"]     = "temperature";
        doc["stat_cla"]    = "measurement";
        doc["val_tpl"]     = "{{ value }}";
        add_device_block(doc);
        add_availability(doc);
        pub_discovery("sensor", "temperature", doc);
    }

    // --- Kelembaban ---
    {
        JsonDocument doc;
        doc["name"]        = "Kelembaban";
        doc["uniq_id"]     = String(FANORA_DEVICE_ID) + "_humidity";
        doc["stat_t"]      = topicStr("sensor/humidity");
        doc["unit_of_meas"]= "%";
        doc["dev_cla"]     = "humidity";
        doc["stat_cla"]    = "measurement";
        doc["val_tpl"]     = "{{ value }}";
        add_device_block(doc);
        add_availability(doc);
        pub_discovery("sensor", "humidity", doc);
    }

    // --- Gerak (PIR) ---
    {
        JsonDocument doc;
        doc["name"]    = "Gerak";
        doc["uniq_id"] = String(FANORA_DEVICE_ID) + "_motion";
        doc["stat_t"]  = topicStr("sensor/motion");
        doc["dev_cla"] = "motion";
        doc["pl_on"]   = "ON";
        doc["pl_off"]  = "OFF";
        add_device_block(doc);
        add_availability(doc);
        pub_discovery("binary_sensor", "motion", doc);
    }

    Serial.println("[HA] Discovery terkirim.");
}
