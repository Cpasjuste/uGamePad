//
// Created by cpasjuste on 07/05/23.
//

#include "main.h"
#include "menu_line.h"
#include "line.h"

using namespace uGamePad;

MenuLine::MenuLine(const Utility::Vec4i &rect) : Rectangle(rect.x, rect.y, rect.w, rect.h, true) {
    MenuLine::setColor(Utility::Color::Black);

    m_name = new Text(2, (int16_t) ((rect.h / 2)), "NAME");
    m_name->setOrigin(Origin::Left);
    MenuLine::add(m_name);

    m_value = new Text((int16_t) (rect.w - 1), (int16_t) ((rect.h / 2)), "VALUE");
    m_value->setOrigin(Origin::Right);
    m_value->setDrawBackground(true);
    MenuLine::add(m_value);
}

void MenuLine::setHighlighted(bool highlighted) {
    if (highlighted) {
        m_value->setColor(Utility::Color::Black);
    } else {
        m_value->setColor(Utility::Color::White);
    }
}

std::string MenuLine::getName() {
    return m_name->getString();
}

void MenuLine::setName(const std::string &name) {
    m_name->setString(name);
}

std::string MenuLine::getValue() {
    return m_value->getString();
}

void MenuLine::setValue(const std::string &value) {
    m_value->setString(value);
}
