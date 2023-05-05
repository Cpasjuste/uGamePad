//
// Created by cpasjuste on 28/03/23.
//

#include "gfx.h"

using namespace uGamePad;

Gfx::Gfx(Utility::Vector2i size) {
    m_size = size;
}

Utility::Vector2i Gfx::getSize() {
    return {p_display->width(), p_display->height()};
}

void Gfx::clear() {
    getDisplay()->fillRect(0, 0, getSize().x, getSize().y, Utility::Color::Black);
}
