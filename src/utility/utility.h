//
// Created by cpasjuste on 25/03/23.
//

#ifndef U_GAMEPAD_UTILITY_H
#define U_GAMEPAD_UTILITY_H

#include <cstdint>
#include <cmath>
#include <string>
#include <vector>
#include "devices.h"

namespace uGamePad {
    class Utility {
    public:
        enum Color {
            Black = 0,
            White = 1,
            Inverse = 2
        };

        struct Vec2f {
            float x;
            float y;

            Vec2f() : x(0), y(0) {
            }

            Vec2f(const float x, const float y) : x(x), y(y) {
            }

            Vec2f operator+(const Vec2f &rhs) const {
                return {x + rhs.x, y + rhs.y};
            }

            Vec2f operator-(const Vec2f &rhs) const {
                return {x - rhs.x, y - rhs.y};
            }

            Vec2f operator*(const float f) const {
                return {x * f, y * f};
            }
        };

        struct Vec2i {
            int16_t x;
            int16_t y;

            Vec2i() : x(0), y(0) {
            }

            Vec2i(const int16_t x, const int16_t y) : x(x), y(y) {
            }

            Vec2i operator+(const Vec2i &rhs) const {
                return {static_cast<int16_t>(x + rhs.x), static_cast<int16_t>(y + rhs.y)};
            }

            Vec2i operator-(const Vec2i &rhs) const {
                return {static_cast<int16_t>(x - rhs.x), static_cast<int16_t>(y - rhs.y)};
            }

            Vec2i operator*(const int16_t i) const {
                return {static_cast<int16_t>(x * i), static_cast<int16_t>(y * i)};
            }
        };

        struct Vec4i {
            int16_t x;
            int16_t y;
            int16_t w;
            int16_t h;

            Vec4i() : x(0), y(0), w(0), h(0) {
            }

            Vec4i(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
                : x(x), y(y), w(w), h(h) {
            }

            Vec4i operator+(const Vec4i &rhs) const {
                return {
                    static_cast<int16_t>(x + rhs.x), static_cast<int16_t>(y + rhs.y),
                    static_cast<int16_t>(w + rhs.w), static_cast<int16_t>(h + rhs.h)
                };
            }

            Vec4i operator-(const Vec4i &rhs) const {
                return {
                    static_cast<int16_t>(x - rhs.x), static_cast<int16_t>(y - rhs.y),
                    static_cast<int16_t>(w - rhs.w), static_cast<int16_t>(h - rhs.h)
                };
            }

            Vec4i operator*(const int16_t i) const {
                return {
                    static_cast<int16_t>(x * i), static_cast<int16_t>(y * i),
                    static_cast<int16_t>(w * i), static_cast<int16_t>(h * i)
                };
            }
        };

        struct Vec6f {
            Vec2f points[3];
        };

        struct Line {
            Vec2f start;
            Vec2f end;
        };

        static void reboot(bool bootloader = false);

        static std::string baseName(const std::string &path);

        static std::string parseButtons(uint32_t buttons);

        static std::string parseSize(uint64_t size);

        // json stuff
        static bool serializeDevice(Device *device, std::vector<uint8_t> *buffer);

        static Device *deserializeDevice(const std::vector<uint8_t> *buffer);
    };
}

#endif //U_GAMEPAD_UTILITY_H
