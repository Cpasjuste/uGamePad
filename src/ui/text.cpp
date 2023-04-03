//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"
#include "text.h"

using namespace uGamePad;

Text::Text(int16_t x, int16_t y, const std::string &str) : Widget() {
    Widget::setPosition(x, y);
    setString(str);
}

Text::Text(const Utility::Vector2i &pos, const std::string &str) : Widget() {
    Widget::setPosition(pos);
    setString(str);
}

void Text::setString(const std::string &str) {
    int16_t x, y;
    uint16_t w, h;
    m_text = str;
    getGfx()->getTextBounds(str.c_str(), m_position.x, m_position.y, &x, &y, &w, &h);
    setSize((int16_t) w, (int16_t) h);
}

void Text::update(const Utility::Vector2i &pos) {
    getGfx()->setCursor(pos.x, pos.y);
    getGfx()->setTextColor(Utility::Color::White);
    getGfx()->print(m_text.c_str());

    Widget::update(pos);
}
