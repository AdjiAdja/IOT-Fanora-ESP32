#ifndef FAN_MQTT_H
#define FAN_MQTT_H

#include <Arduino.h>
#include "config.h"

typedef void (*CommandCallback)(const char* topic, const char* payload);

void mqtt_init();
void mqtt_update();
bool mqtt_is_connected();

void mqtt_publish_button(uint8_t index, bool pressed);
void mqtt_publish_event(uint8_t index, const char* eventType);
void mqtt_publish_temperature(float t);
void mqtt_publish_humidity(float h);
void mqtt_publish_motion(bool motion);
void mqtt_publish_status(const char* status);

void mqtt_send_discovery();
void mqtt_set_command_callback(CommandCallback cb);

#endif // FAN_MQTT_H
