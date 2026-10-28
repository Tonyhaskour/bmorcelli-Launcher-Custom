# ESP32-S3 N16R8 + ST7735 Custom Board

## Modules sharing SPI bus 1
| Device  | SCK   | MISO  | MOSI  | CS    |
| :---    | :---: | :---: | :---: | :---: |
| Display | 40    | -     | 42    | 45    |
| SDCard  | 40    | 41    | 42    | 1     |

## Modules sharing I2C bus
*None used in this configuration.*

## Navigation Inputs (6 Buttons)
| Function | GPIO |
| :---     | :---: |
| Up       | 38   |
| Down     | 39   |
| Left     | 4    |
| Right    | 5    |
| Select (OK/A) | 15 |
| Esc (Back/B)  | 16 |

## Device specific initialization
- Backlight (TFT_BL) is on GPIO 2, driven via PWM.
- All CS pins (Display GPIO 45, SDCard GPIO 1) default HIGH to avoid bus contention.
