//
// Created by cpasjuste on 30/03/23.
//

#include "main.h"
#include "bitmap.h"

using namespace uGamePad;

Bitmap::Bitmap(const Utility::Vec2i &pos, const Utility::Vec2i &size, uint8_t *bitmap) : Widget() {
    m_bitmap = bitmap;
    Widget::setPosition(pos);
    Widget::setSize(size);
}

void Bitmap::loop(const Utility::Vec2i &pos) {
    getGfx()->drawBitmap(pos.x, pos.y, m_bitmap, m_size.x, m_size.y, m_color);
    Widget::loop(pos);
}
