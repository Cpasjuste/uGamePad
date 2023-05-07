//
// Created by cpasjuste on 03/04/23.
//

#include "main.h"
#include "circle.h"

using namespace uGamePad;

Circle::Circle(const Utility::Vec2i &pos, int16_t size, bool fill) : Widget() {
    Widget::setPosition(pos);
    Widget::setSize(size, size);
    m_fill = fill;
}

void Circle::loop(const Utility::Vec2i &pos) {
    if (m_fill) {
        getGfx()->fillCircle(pos.x, pos.y, m_size.x, m_color);
    } else {
        getGfx()->drawCircle(pos.x, pos.y, m_size.x, m_color);
    }

    Widget::loop(pos);
}
