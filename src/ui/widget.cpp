//
// Created by cpasjuste on 30/03/23.
//

#include <algorithm>
#include "main.h"
#include "platform.h"
#include "widget.h"

using uGamePad::Utility;
using uGamePad::Widget;

void Widget::add(uGamePad::Widget *widget) {
    if (widget) {
        widget->p_parent = this;
        p_child_list.push_back(widget);
    }
}

void Widget::remove(uGamePad::Widget *widget) {
    if (!p_child_list.empty()) {
        p_child_list.erase(std::remove(p_child_list.begin(), p_child_list.end(), widget), p_child_list.end());
    }
}

void Widget::setPosition(int16_t x, int16_t y) {
    m_position.x = x;
    m_position.y = y;
}

void Widget::setPosition(const Utility::Vector2 &pos) {
    setPosition(pos.x, pos.y);
}

Utility::Vector2 Widget::getPosition() {
    return m_position;
}

void Widget::setSize(int16_t x, int16_t y) {
    m_size.x = x;
    m_size.y = y;
}

void Widget::setSize(const Utility::Vector2 &size) {
    m_size.x = size.x;
    m_size.y = size.y;
}

Adafruit_GFX *uGamePad::Widget::getGfx() {
    return getPlatform()->getGfx()->getDisplay();
}

void Widget::update(const Utility::Vector2 &pos) {
    for (auto &widget: p_child_list) {
        if (widget) {
            Utility::Vector2 v = {(int16_t) (pos.x + widget->m_position.x),
                                  (int16_t) (pos.y + widget->m_position.y)};
            widget->update(v);
        }
    }
}

uGamePad::Widget::~Widget() {
    for (auto widget = p_child_list.begin(); widget != p_child_list.end();) {
        if (*widget) {
            delete (*widget);
        }
    }
    p_child_list.clear();
    if (p_parent) {
        p_parent->remove(this);
    }
}
