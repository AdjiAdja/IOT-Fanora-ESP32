#ifndef FAN_SENSORS_H
#define FAN_SENSORS_H

#include <Arduino.h>
#include "config.h"

void  sensors_init();
void  sensors_update();
float sensors_get_temperature();
float sensors_get_humidity();
bool  sensors_get_motion();
bool  sensors_motion_changed();
bool  sensors_has_new_reading();

#endif // FAN_SENSORS_H
