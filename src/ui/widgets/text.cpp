//
// Created by cpasjuste on 30/03/23.
//

#include "main.h"
#include "text.h"

using namespace uGamePad;

Text::Text(int16_t x, int16_t y, const std::string &str, const Utility::Color &color) : Widget() {
    m_color = color;
    m_bg_rect = new Rectangle(-1, -1, 8, 8, true, 0);
    m_bg_rect->setVisibility(Visibility::Hidden);
    Text::add(m_bg_rect);
    Widget::setPosition(x, y);
    Text::getGfx()->setTextWrap(false);
    setString(str);
}

Text::Text(const Utility::Vector2i &pos, const std::string &str, const Utility::Color &color) : Widget() {
    m_color = color;
    m_bg_rect = new Rectangle(-1, -1, 8, 8, true, 0);
    m_bg_rect->setVisibility(Visibility::Hidden);
    Text::add(m_bg_rect);
    Widget::setPosition(pos);
    Text::getGfx()->setTextWrap(false);
    setString(str);
}

void Text::setString(const std::string &str) {
    int16_t x, y;
    uint16_t w, h;
    m_text = str;
    getGfx()->getTextBounds(str.c_str(), m_position.x, m_position.y, &x, &y, &w, &h);
    setSize((int16_t) w, (int16_t) h);
    m_bg_rect->setSize(w, h);
}

void Text::setDrawBackground(bool drawBg) {
    m_bg_rect->setVisibility(drawBg ? Visibility::Visible : Visibility::Hidden);
}

void Text::loop(const Utility::Vector2i &pos) {
    // draw child first (bg)
    if (m_bg_rect->isVisible()) {
        if (m_color == Utility::Color::White) {
            m_bg_rect->setColor(Utility::Color::Black);
        } else {
            m_bg_rect->setColor(Utility::Color::White);
        }
    }
    Widget::loop(pos);

    // now draw the text
    getGfx()->setCursor(pos.x, pos.y);
    getGfx()->setTextColor(m_color);
    getGfx()->print(m_text.c_str());
}
