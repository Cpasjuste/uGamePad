//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"
#include "text.h"

using namespace uGamePad;

Text::Text(int16_t x, int16_t y, const std::string &text) : Widget() {
    Widget::setPosition(x, y);
    m_text = text;
}

Text::Text(const Utility::Vector2i &pos, const std::string &text) : Widget() {
    Widget::setPosition(pos);
    m_text = text;
}

void Text::update(const Utility::Vector2i &pos) {
    if (isVisible()) {
        getGfx()->setCursor(pos.x, pos.y);
        getGfx()->setTextColor(Utility::Color::White);
        getGfx()->print(m_text.c_str());
    }

    Widget::update(pos);
}
