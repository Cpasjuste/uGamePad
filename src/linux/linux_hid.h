//
// Created by cpasjuste on 11/03/24.
//

#ifndef U_GAMEPAD_LINUX_HID_H
#define U_GAMEPAD_LINUX_HID_H

namespace uGamePad {
    class LinuxHid : public Hid {
    public:
        LinuxHid();

        ~LinuxHid();

        void loop() override;

    private:
        bool m_inited = false;
    };
}

#endif //U_GAMEPAD_LINUX_HID_H
