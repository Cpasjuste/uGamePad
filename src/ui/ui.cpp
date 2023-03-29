//
// Created by cpasjuste on 28/03/23.
//

#include "ui.h"

using uGamePad::Ui;

extern unsigned char splash_128x64 PROGMEM[];

Ui::Ui(Vector2 size) {
    m_size = size;
}



void Ui::drawSplash() {
    p_display->drawBitmap(0, 0, splash_128x64, m_size.x, m_size.y, getColor(White));
    p_display->setTextColor(getColor(White));
    p_display->setCursor(42, 56);
    p_display->print("uGamePad");
    flip();
}

uint16_t Ui::getColor(uGamePad::Ui::Color color) {
    switch (color) {
        case Black:
            return 0;
        case White:
            return 1;
        case Inverse:
            return 2;
    }

    return 0;
}
