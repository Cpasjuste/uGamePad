//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"
#include "line.h"

using namespace uGamePad;

Line::Line(const Utility::Vector2i &pos, int16_t length, float rotation) : Widget() {
    Widget::setPosition(pos);
    Widget::setSize(length, length);
    m_rotation = rotation;
    m_line = calculateLine({(float) pos.x, (float) pos.y}, (float) length, m_rotation);
}

Utility::Line Line::calculateLine(Utility::Vector2f position, float length, float rotation) {
    Utility::Line line{};
    Utility::Vector2f start = {0.0f, 0.0f};
    Utility::Vector2f end = {length, 0.0f};

    // Convert rotation to radians
    float radians = rotation * (float) M_PI / 180.0f;

    // Calculate new x and y coordinates for start and end points
    float startX = position.x + cosf(radians) * start.x - sinf(radians) * start.y;
    float startY = position.y + sinf(radians) * start.x + cosf(radians) * start.y;
    float endX = position.x + cosf(radians) * end.x - sinf(radians) * end.y;
    float endY = position.y + sinf(radians) * end.x + cosf(radians) * end.y;

    // Round coordinates to nearest pixel
    start.x = std::round(startX);
    start.y = std::round(startY);
    end.x = std::round(endX);
    end.y = std::round(endY);

    // Clamp to screen bounds
    start.x = fmaxf(0.0f, fminf(start.x, (float) getGfx()->width() - 1.0f));
    start.y = fmaxf(0.0f, fminf(start.y, (float) getGfx()->height() - 1.0f));
    end.x = fmaxf(0.0f, fminf(end.x, (float) getGfx()->width() - 1.0f));
    end.y = fmaxf(0.0f, fminf(end.y, (float) getGfx()->height() - 1.0f));

    line.start = start;
    line.end = end;

    return line;
}

void Line::update(const Utility::Vector2i &pos) {
    if (m_rotation == 0 || m_rotation == 180 || m_rotation == 360) {
        getGfx()->drawFastHLine((int16_t) m_line.start.x, (int16_t) m_line.start.y,
                                m_size.x, m_color);
    } else if (m_rotation == 90 || m_rotation == 270) {
        getGfx()->drawFastVLine((int16_t) m_line.start.x, (int16_t) m_line.start.y,
                                m_size.x, m_color);
    } else {
        getGfx()->drawLine((int16_t) m_line.start.x, (int16_t) m_line.start.y,
                           (int16_t) m_line.end.x, (int16_t) m_line.end.y, m_color);
    }

    Widget::update(pos);
}