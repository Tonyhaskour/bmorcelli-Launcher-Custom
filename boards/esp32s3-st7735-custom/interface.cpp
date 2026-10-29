#include "idf/launcher_platform.h"
#include "powerSave.h"
#include <interface.h>

// Include the Launcher's Hardware Abstraction Layer (HAL) for buttons and backlight
#include "hal/device.h"
#include "hal/inputs/buttons.h"
#include "hal/bright/bright.h"

// Configure your 6 buttons to match Retro-Go
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

/***************************************************************************************
** Function:    _setup_gpio()
** Description: initial setup for the device
***************************************************************************************/
void _setup_gpio() {
    // Keep the SD card chip-select high so it ignores display traffic
    pinMode(SDCARD_CS, OUTPUT);
    digitalWrite(SDCARD_CS, HIGH);

    // Initialize the 6 buttons
    hal_buttons_init(buttonsCfg(), 6);
}

/***************************************************************************************
** Function:    _post_setup_gpio()
** Description: second stage gpio setup, run after TFT and before SD card initialization
***************************************************************************************/
void _post_setup_gpio() {
    // Attach the backlight to the hardware PWM driver
    hal_bright_attach(TFT_BL);
}

/***************************************************************************************
** Function: _late_setup_gpio()
** Description: third stage gpio, run before bootscreen animation
***************************************************************************************/
void _late_setup_gpio() {}

/***************************************************************************************
** Function name: getBattery()
** Description:   Delivers the battery value from 1-100
***************************************************************************************/
int getBattery() {
    return 0;
}

/***************************************************************************************
** Function name: isCharging()
** Description:   true while charging: draws a bolt on the battery icon.
***************************************************************************************/
bool isCharging() {
    return false;
}

/*********************************************************************
** Function: setBrightness
** Description: set brightness value
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    hal_bright_set(TFT_BL, brightval);
}

/*********************************************************************
** Function: InputHandler
** Handles the variables PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**********************************************************************/
void InputHandler(void) {
    // Poll the 6 buttons and map them to the Launcher's UI actions
    hal_buttons_poll_6(buttonsCfg());
    return;
}

/*********************************************************************
** Function: powerOff
** Description: Turns off the device
**********************************************************************/
void powerOff() {
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
    esp_deep_sleep_start();
}

/*********************************************************************
** Function: reboot
** Description: Handles reboot process
**********************************************************************/
void reboot() {
    ESP.restart();
}
