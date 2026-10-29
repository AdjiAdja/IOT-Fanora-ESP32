#ifndef FAN_BUTTONS_H
#define FAN_BUTTONS_H

#include <Arduino.h>
#include "config.h"

struct ButtonState {
    uint8_t       pin;
    bool          lastReading;
    bool          stableState;
    unsigned long lastChangeMs;
    unsigned long pressCount;
};

void          buttons_init();
void          buttons_update();
bool          buttons_is_pressed(uint8_t index);
bool          buttons_was_toggled(uint8_t index);
unsigned long buttons_get_press_count(uint8_t index);

#endif // FAN_BUTTONS_H
