//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PLATFORM_H
#define U_GAMEPAD_PLATFORM_H

#include "gamepad.h"
#include "gfx.h"
#include "ui.h"
#include "fs.h"

namespace uGamePad {
    class Platform {
    public:
        Platform() = default;

        virtual void setup();

        virtual void loop();

        virtual Gfx *getGfx() { return p_gfx; };

        virtual GamePad *getPad() { return p_pad; };

        virtual Ui *getUi() { return p_ui; };

        virtual Fs *getFs() { return p_fs; };

    protected:
        Gfx *p_gfx = nullptr;
        GamePad *p_pad = nullptr;
        Ui *p_ui = nullptr;
        Fs *p_fs = nullptr;
    };
}

#endif //U_GAMEPAD_PLATFORM_H
