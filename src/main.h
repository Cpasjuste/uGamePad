//
// Created by cpasjuste on 23/03/23.
//

#ifndef U_GAMEPAD_MAIN_H
#define U_GAMEPAD_MAIN_H

#include "gamepad.h"
#include "fs.h"
#include "gfx.h"
#include "ui.h"
#include "platform.h"

#if NATIVE
#include "linux_platform.h"
#include "linux_gamepad.h"
#include "linux_gfx.h"
#include "fs.h"
#else
#include "pico_platform.h"
#include "pico_gamepad.h"
#include "pico_gfx.h"
#include "pico_fs.h"
#endif

extern uGamePad::Platform *getPlatform();

#endif //U_GAMEPAD_MAIN_H
