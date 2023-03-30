//
// Created by cpasjuste on 28/03/23.
//

#include "gfx.h"

using uGamePad::Gfx;
using uGamePad::Utility;

//extern unsigned char splash_128x64 PROGMEM[];

Gfx::Gfx(Utility::Vector2 size) {
    m_size = size;
}

Utility::Vector2 Gfx::getSize() {
    return {p_display->width(), p_display->height()};
}

/*
void Gfx::drawSplash() {
    p_gfx->drawBitmap(0, 0, splash_128x64, m_size.x, m_size.y, getColor(White));
    p_gfx->setTextColor(getColor(White));
    p_gfx->setCursor(42, 56);
    p_gfx->print("uGamePad");
    flip();
}
*/