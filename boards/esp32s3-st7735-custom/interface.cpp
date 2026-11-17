#include "idf/launcher_platform.h"
#include "powerSave.h"
#include <interface.h>

// Include the Launcher's Hardware Abstraction Layer (HAL) for buttons and backlight
#include "hal/device.h"
#include "hal/inputs/buttons.h"
#include "hal/bright/bright.h"

// Configure your 6 buttons
static DeviceButtons buttonsCfg() {
    // Order: {Prev/Left, Next/Right, Up, Down, Sel, Esc}
    DeviceButtons cfg{
        4,   // Left / Prev
        5,   // Right / Next
        38,  // Up
        39,  // Down
        15,  // Select (Retro-Go A)
        16   // Esc (Retro-Go B)
    };
    return cfg;
}

void _setup_gpio() {
    // Keep the SD card chip-select high so it ignores display traffic
    pinMode(SDCARD_CS, OUTPUT);
    digitalWrite(SDCARD_CS, HIGH);

    // Initialize the 6 buttons using the HAL
    hal_buttons_init(buttonsCfg(), 6);
}

void _post_setup_gpio() {
    // Attach the backlight to the hardware PWM driver
    hal_bright_attach(TFT_BL);
}

void _late_setup_gpio() {}

int getBattery() { return 0; }
bool isCharging() { return false; }

void _setBrightness(uint8_t brightval) {
    hal_bright_set(TFT_BL, brightval);
}

void InputHandler(void) {
    // Poll the 6 buttons using the HAL (handles long-presses for the keyboard)
    hal_buttons_poll_6(buttonsCfg());
}

void powerOff() {
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
    esp_deep_sleep_start();
}

void reboot() {
    ESP.restart();
}
