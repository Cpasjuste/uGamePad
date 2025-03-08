//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PLATFORM_H
#define U_GAMEPAD_PLATFORM_H

#include "fs.h"
#include "config.h"
#include "gamepad.h"
#include "hid.h"
#include "gfx.h"
#include "ui.h"

namespace uGamePad {
    class Platform {
    public:
        Platform() = default;

        ~Platform();

        virtual void setup();

        virtual void loop();

        virtual Fs *getFs() { return p_fs; }

        virtual Config *getConfig() { return p_config; }

        virtual Hid *getHid() { return p_hid; }

        virtual GamePad *getPad() { return p_pad; }

        virtual Gfx *getGfx() { return p_gfx; }

        virtual Ui *getUi() { return p_ui; }

        virtual int getFreeHeap() { return 0; }

    protected:
        Gfx *p_gfx = nullptr;
        GamePad *p_pad = nullptr;
        Ui *p_ui = nullptr;
        Fs *p_fs = nullptr;
        Config *p_config = nullptr;
        Hid *p_hid = nullptr;
    };
}

#endif //U_GAMEPAD_PLATFORM_H
