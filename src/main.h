//
// Created by cpasjuste on 23/03/23.
//

#ifndef U_GAMEPAD_MAIN_H
#define U_GAMEPAD_MAIN_H

#define VERSION "1.1.0"

#define WIP_DISABLE_OPTION_REMAP
#define WIP_DISABLE_OPTION_AUTO_FIRE

#include "platform.h"

#if LINUX
#include "linux_platform.h"
#include "linux_gamepad.h"
#include "linux_gfx.h"
#include "linux_fs.h"
#else
#include "pico_platform.h"
#endif

extern uGamePad::Platform *getPlatform();

#endif //U_GAMEPAD_MAIN_H
