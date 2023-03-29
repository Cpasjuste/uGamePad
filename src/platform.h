//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PLATFORM_H
#define U_GAMEPAD_PLATFORM_H

#include "gamepad.h"
#include "ui.h"

namespace uGamePad {
    class Platform {
    public:
        virtual void loop() {};

        virtual Ui *getUi() { return p_ui; };

        virtual GamePad *getPad() { return p_pad; };

    protected:
        Ui *p_ui = nullptr;
        GamePad *p_pad = nullptr;
    };
}

#endif //U_GAMEPAD_PLATFORM_H
