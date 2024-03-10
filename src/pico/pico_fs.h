//
// Created by cpasjuste on 10/03/24.
//

#ifndef U_GAMEPAD_PICO_FS_H
#define U_GAMEPAD_PICO_FS_H

#include "fs.h"

namespace uGamePad {
    class PicoFs : public Fs {
    public:
        void setUsbMode(UsbMode mode) override;
    };
}

#endif //U_GAMEPAD_PICO_FS_H
