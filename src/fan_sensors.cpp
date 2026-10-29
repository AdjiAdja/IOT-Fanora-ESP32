#include "fan_sensors.h"
#include <DHT.h>

static DHT           dht(PIN_DHT, DHT_TYPE);
static float         lastTemp   = 0.0f;
static float         lastHum    = 0.0f;
static bool          motionState = false;
static bool          motionEdge  = false;
static unsigned long lastReadMs  = 0;
static bool          newReading  = false;

void sensors_init() {
    dht.begin();
    pinMode(PIN_PIR, INPUT);
    lastReadMs = 0;
    motionState = digitalRead(PIN_PIR) == HIGH;
    motionEdge  = false;
}

void sensors_update() {
    // --- PIR: baca setiap loop ---
    bool motion = digitalRead(PIN_PIR) == HIGH;
    if (motion != motionState) {
        motionState = motion;
        motionEdge  = true;
    }

    // --- DHT22: baca periodik ---
    unsigned long now = millis();
    if (now - lastReadMs >= SENSOR_INTERVAL_MS) {
        lastReadMs = now;
        float t = dht.readTemperature();
        float h = dht.readHumidity();
        if (!isnan(t)) { lastTemp = t; newReading = true; }
        if (!isnan(h)) { lastHum  = h; newReading = true; }
    }
}

float sensors_get_temperature() { return lastTemp; }
float sensors_get_humidity()    { return lastHum; }
bool  sensors_get_motion()      { return motionState; }

bool sensors_motion_changed() {
    bool e = motionEdge;
    motionEdge = false;
    return e;
}

bool sensors_has_new_reading() {
    bool r = newReading;
    newReading = false;
    return r;
}
