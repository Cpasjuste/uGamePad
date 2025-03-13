//
// Created by cpasjuste on 14/03/24.
//

#ifndef U_GAMEPAD_PICO_HID_H
#define U_GAMEPAD_PICO_HID_H

namespace uGamePad {
    class PicoHid final : public Hid {
    public:
        ~PicoHid() override = default;

        void loop() override;

    private:
        bool m_inited = false;
        Clock m_clock;
    };
}

#endif //U_GAMEPAD_PICO_HID_H
