#include "fan_buttons.h"

static ButtonState buttons[BUTTON_COUNT];
static bool        toggledFlag[BUTTON_COUNT];

static const uint8_t buttonPins[BUTTON_COUNT] = {
    PIN_BTN_1, PIN_BTN_2, PIN_BTN_3, PIN_BTN_4,
    PIN_BTN_5, PIN_BTN_6, PIN_BTN_7, PIN_BTN_8
};

void buttons_init() {
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
        buttons[i]       = { buttonPins[i], true, true, 0, 0 };
        toggledFlag[i]   = false;
    }
}

void buttons_update() {
    unsigned long now = millis();
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        bool reading = digitalRead(buttons[i].pin);

        if (reading != buttons[i].lastReading) {
            buttons[i].lastChangeMs = now;
            buttons[i].lastReading  = reading;
        }

        if ((now - buttons[i].lastChangeMs) > DEBOUNCE_MS) {
            if (reading != buttons[i].stableState) {
                buttons[i].stableState = reading;
                if (reading == LOW) {          // falling edge = ditekan
                    toggledFlag[i] = true;
                    buttons[i].pressCount++;
                }
            }
        }
    }
}

bool buttons_is_pressed(uint8_t i) {
    return (i < BUTTON_COUNT) && (buttons[i].stableState == LOW);
}

bool buttons_was_toggled(uint8_t i) {
    if (i >= BUTTON_COUNT) return false;
    bool flag = toggledFlag[i];
    toggledFlag[i] = false;
    return flag;
}

unsigned long buttons_get_press_count(uint8_t i) {
    return (i < BUTTON_COUNT) ? buttons[i].pressCount : 0;
}
