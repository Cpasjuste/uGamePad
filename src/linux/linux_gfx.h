//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_LINUX_GFX_H
#define U_GAMEPAD_LINUX_GFX_H

namespace uGamePad {
    class LinuxGfx : public Gfx {
    public:
        LinuxGfx();

        void flip() override;
    };
}

#endif //U_GAMEPAD_LINUX_GFX_H
