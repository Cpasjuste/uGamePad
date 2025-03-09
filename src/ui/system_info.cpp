//
// Created by cpasjuste on 09/03/25.
//

#include "main.h"
#include "system_info.h"

using namespace uGamePad;

SystemInfo::SystemInfo(const Utility::Vec2i &pos, const Utility::Vec2i &size) : Rectangle(pos, size) {
}

void SystemInfo::loop(const Utility::Vec2i &pos) {
    Rectangle::loop(pos);
}
