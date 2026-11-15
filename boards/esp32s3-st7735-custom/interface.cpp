#include "idf/launcher_platform.h"
#include "powerSave.h"
#include <interface.h>

#include "hal/device.h"
#include "hal/bright/bright.h"

// CRITICAL FIX: Increase the loop task stack size for Wi-Fi and Keyboard operations
// This replaces the ineffective CONFIG_ESP_MAIN_TASK_STACK_SIZE flag.
SET_LOOP_TASK_STACK_SIZE(24 * 1024);

// Button Pin Definitions (Order: Prev/Left, Next/Right, Up, Down, Sel, Esc)
static const uint8_t BTN_PINS[6] = {4, 5, 38, 39, 15, 16};

#define DEBOUNCE_MS      25
#define REPEAT_FIRST_MS  400
#define REPEAT_MS        150

void _setup_gpio() {
    // Keep SD Card CS high to prevent bus conflicts
    pinMode(SDCARD_CS, OUTPUT);
    digitalWrite(SDCARD_CS, HIGH);

    // Initialize button pins with internal pull-ups
    for (uint8_t i = 0; i < 6; i++) {
        pinMode(BTN_PINS[i], INPUT_PULLUP);
    }
}

void _post_setup_gpio() {
    hal_bright_attach(TFT_BL);
}

void _late_setup_gpio() {}

int getBattery() { return 0; }
bool isCharging() { return false; }

void _setBrightness(uint8_t brightval) {
    hal_bright_set(TFT_BL, brightval);
}

void InputHandler(void) {
    static bool     raw[6], stable[6];
    static uint32_t changed[6], nextFire[6];
    const uint32_t now = millis();
    bool fire[6] = {false};
    bool any = false;

    // Software Debounce and Auto-repeat logic
    for (uint8_t i = 0; i < 6; i++) {
        bool r = (digitalRead(BTN_PINS[i]) == LOW);   // Active Low
        if (r != raw[i]) { raw[i] = r; changed[i] = now; }

        if (now - changed[i] >= DEBOUNCE_MS && stable[i] != raw[i]) {
            stable[i] = raw[i];
            if (stable[i]) { fire[i] = true; nextFire[i] = now + REPEAT_FIRST_MS; }
        } else if (stable[i] && (int32_t)(now - nextFire[i]) >= 0) {
            fire[i] = true;                            // Auto-repeat while held
            nextFire[i] = now + REPEAT_MS;
        }
        any |= fire[i];
    }

    if (!any) return;
    
    // Wake screen on any press, swallowing the first press to prevent accidental inputs
    if (wakeUpScreen()) return;   
    
    AnyKeyPress = true;

    if (fire[0]) PrevPress = true;
    if (fire[1]) NextPress = true;
    if (fire[2]) UpPress   = true;
    if (fire[3]) DownPress = true;
    if (fire[4]) SelPress  = true;
    if (fire[5]) EscPress  = true;
}

void powerOff() {
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
    esp_deep_sleep_start();
}

void reboot() {
    ESP.restart();
}
