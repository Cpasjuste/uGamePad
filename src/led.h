//
// Created by cpasjuste on 10/04/25.
//

#ifndef LED_H
#define LED_H

namespace uGamePad {
    class Led {
    public:
        Led() = default;

        virtual ~Led() = default;

        virtual void put(uint32_t pixel) {
        }

        virtual void put(const uint8_t r, const uint8_t g, const uint8_t b) {
            put(static_cast<uint32_t>(r) << 8
                | static_cast<uint32_t>(g) << 16
                | static_cast<uint32_t>(b));
        }
    };
}

#endif //LED_H
