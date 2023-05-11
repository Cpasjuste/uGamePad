//
// Created by cpasjuste on 09/05/23.
//

#ifndef U_GAMEPAD_JSON_H
#define U_GAMEPAD_JSON_H

#ifdef NATIVE
#undef ARDUINO
#define ARDUINOJSON_ENABLE_STD_STREAM 1
#endif
#include <ArduinoJson.h>

namespace uGamePad {
    class Json {
    public:
        static std::vector<uint8_t> getDevice(Device *device);

        static Device *getDevice(const std::vector<uint8_t> &buffer);
    };

} // uGamePad

#endif //U_GAMEPAD_JSON_H
