//
// Created by cpasjuste on 30/03/23.
//

#include <cmath>
#include "platform.h"
#include "triangle.h"

using namespace uGamePad;

Triangle::Triangle(const Utility::Vector2i &pos, uint16_t base, uint16_t height,
                   uint8_t rotation, bool fill) : Widget() {
    Widget::setPosition(pos);
    Widget::setSize({(int16_t) base, (int16_t) height});
    m_rotation = rotation;
    m_fill = fill;
    m_triangle = calculateTriangle(pos, rotation, base, height);
}

void Triangle::setPosition(int16_t x, int16_t y) {
    Widget::setPosition(x, y);
    m_triangle = calculateTriangle(m_position, m_rotation, m_size.x, m_size.y);
}

void Triangle::setRotation(float rotation) {
    m_rotation = rotation;
    m_triangle = calculateTriangle(m_position, m_rotation, m_size.x, m_size.y);
}

Utility::Vector2f Triangle::rotate(Utility::Vector2f point, float rotation) {
    Utility::Vector2f result{};
    result.x = point.x * cos(rotation) - point.y * sin(rotation);
    result.y = point.x * sin(rotation) + point.y * cos(rotation);
    return result;
}

Utility::Vector6f Triangle::calculateTriangle(Utility::Vector2i position, float rotation, float base, float height) {
    Utility::Vector6f triangle{};
    Utility::Vector2f point1 = {0.0f, 0.0f};
    Utility::Vector2f point2 = {base, 0.0f};
    Utility::Vector2f point3 = {base / 2.0f, height};

    // Rotate points
    point1 = rotate(point1, rotation);
    point2 = rotate(point2, rotation);
    point3 = rotate(point3, rotation);

    // Translate points
    point1.x += position.x;
    point1.y += position.y;
    point2.x += position.x;
    point2.y += position.y;
    point3.x += position.x;
    point3.y += position.y;

    triangle.points[0] = point1;
    triangle.points[1] = point2;
    triangle.points[2] = point3;

    return triangle;
}

void Triangle::update(const Utility::Vector2i &pos) {
    if (isVisible()) {
        if (m_fill) {
            getGfx()->fillTriangle((int16_t) m_triangle.points[0].x, (int16_t) m_triangle.points[0].y,
                                   (int16_t) m_triangle.points[1].x, (int16_t) m_triangle.points[1].y,
                                   (int16_t) m_triangle.points[2].x, (int16_t) m_triangle.points[2].y,
                                   Utility::Color::White);
        } else {
            getGfx()->drawTriangle((int16_t) m_triangle.points[0].x, (int16_t) m_triangle.points[0].y,
                                   (int16_t) m_triangle.points[1].x, (int16_t) m_triangle.points[1].y,
                                   (int16_t) m_triangle.points[2].x, (int16_t) m_triangle.points[2].y,
                                   Utility::Color::White);
        }
    }

    Widget::update(pos);
}
