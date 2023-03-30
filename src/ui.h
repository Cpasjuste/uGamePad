//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_UI_H
#define U_GAMEPAD_UI_H

#include <cstdint>

namespace uGamePad {
    class Ui {
    public:
        explicit Ui();

        virtual void loop();
    };
}

#endif //U_GAMEPAD_UI_H
