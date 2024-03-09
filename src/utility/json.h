//
// Created by cpasjuste on 09/05/23.
//

#ifndef U_GAMEPAD_JSON_H
#define U_GAMEPAD_JSON_H

#define ARDUINOJSON_ENABLE_PROGMEM 0
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 0
#define ARDUINOJSON_ENABLE_ARDUINO_PRINT 0
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 0
#include "ArduinoJson-v7.0.3.h"
#include "devices.h"

namespace uGamePad {
    class Json {
    public:
        static std::vector<uint8_t> getDevice(Device *device);

        static Device *getDevice(const std::vector<uint8_t> &buffer);
    };

} // uGamePad

#endif //U_GAMEPAD_JSON_H
