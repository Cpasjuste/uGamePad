//
// Created by cpasjuste on 30/03/23.
//

#include "main.h"
#include "rectangle.h"

using namespace uGamePad;

Rectangle::Rectangle(int16_t x, int16_t y, int16_t w, int16_t h,
                     bool fill, int16_t radius, const Utility::Color &color) : Widget() {
    Widget::setPosition(x, y);
    Widget::setSize(w, h);
    m_color = color;
    m_fill = fill;
    m_radius = radius;
}

Rectangle::Rectangle(const Utility::Vector2i &pos, const Utility::Vector2i &size,
                     bool fill, int16_t radius, const Utility::Color &color) : Widget() {
    Widget::setPosition(pos);
    Widget::setSize(size);
    m_color = color;
    m_fill = fill;
    m_radius = radius;
}

void Rectangle::update(const Utility::Vector2i &pos) {
    if (m_fill) {
        if (m_radius > 0) {
            getGfx()->fillRoundRect(pos.x, pos.y, m_size.x, m_size.y, m_radius, m_color);
        } else {
            getGfx()->fillRect(pos.x, pos.y, m_size.x, m_size.y, m_color);
        }
    } else {
        if (m_radius > 0) {
            getGfx()->drawRoundRect(pos.x, pos.y, m_size.x, m_size.y, m_radius, m_color);
        } else {
            getGfx()->drawRect(pos.x, pos.y, m_size.x, m_size.y, m_color);
        }
    }

    Widget::update(pos);
}
