//
// Created by cpasjuste on 23/03/23.
//

#ifndef USBH_GAMEPAD_MAIN_H
#define USBH_GAMEPAD_MAIN_H

#include "led.h"

// On SAMD boards where the native USB port is also the serial console, use
// Serial1 for the serial console. This applies to all SAMD boards except for
// Arduino Zero and M0 boards.
#if (USB_VID == 0x2341 && defined(ARDUINO_SAMD_ZERO)) || (USB_VID == 0x2a03 && defined(ARDUINO_SAM_ZERO))
#define SerialDebug SERIAL_PORT_MONITOR
#else
#define Debug Serial1
#endif

#endif //USBH_GAMEPAD_MAIN_H
