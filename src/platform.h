//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PLATFORM_H
#define U_GAMEPAD_PLATFORM_H

//#include "fs.h"
//#include "config.h"
#include "gamepad.h"
#include "gfx.h"
#include "ui.h"

namespace uGamePad {
    class Platform {
    public:
        Platform() = default;

        virtual void setup();

        virtual void loop();

        virtual Gfx *getGfx() { return p_gfx; };

        virtual GamePad *getPad() { return p_pad; };

        virtual Ui *getUi() { return p_ui; };

        //virtual Fs *getFs() { return p_fs; };

        //virtual Config *getConfig() { return p_config; };

        virtual int getFreeHeap() { return 0; };

    protected:
        Gfx *p_gfx = nullptr;
        GamePad *p_pad = nullptr;
        Ui *p_ui = nullptr;
        //Fs *p_fs = nullptr;
        //Config *p_config = nullptr;
    };
}

#endif //U_GAMEPAD_PLATFORM_H
