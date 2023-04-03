//
// Created by cpasjuste on 30/03/23.
//

#include <cmath>
#include "platform.h"
#include "triangle.h"

using namespace uGamePad;

Triangle::Triangle(const Utility::Vector2i &pos, uint16_t size, float rotation, bool fill) : Widget() {
    Widget::setPosition(pos);
    Widget::setSize({(int16_t) size, (int16_t) size});
    m_rotation = rotation;
    m_fill = fill;
    m_triangle = calculateTriangle(pos, (float) size, rotation);
}

void Triangle::setPosition(int16_t x, int16_t y) {
    Widget::setPosition(x, y);
    m_triangle = calculateTriangle(m_position, m_size.x, m_rotation);
}

void Triangle::setRotation(float rotation) {
    m_rotation = rotation;
    m_triangle = calculateTriangle(m_position, m_size.x, m_rotation);
}

Utility::Vector6f Triangle::calculateTriangle(Utility::Vector2i position, float size, float rotation) {
    Utility::Vector6f triangle{};
    Utility::Vector2f v1 = {-size / 2.0f, -size / 2.0f};
    Utility::Vector2f v2 = {size / 2.0f, -size / 2.0f};
    Utility::Vector2f v3 = {0.0f, size / 2.0f};

    // Convert rotation to radians
    float radians = rotation * (float) M_PI / 180.0f;

    // Rotate vertices
    float cosTheta = cosf(radians);
    float sinTheta = sinf(radians);
    Utility::Vector2f v1r = {v1.x * cosTheta - v1.y * sinTheta, v1.x * sinTheta + v1.y * cosTheta};
    Utility::Vector2f v2r = {v2.x * cosTheta - v2.y * sinTheta, v2.x * sinTheta + v2.y * cosTheta};
    Utility::Vector2f v3r = {v3.x * cosTheta - v3.y * sinTheta, v3.x * sinTheta + v3.y * cosTheta};

    // Translate vertices to position
    Utility::Vector2f p1 = {(float) position.x + v1r.x, (float) position.y + v1r.y};
    Utility::Vector2f p2 = {(float) position.x + v2r.x, (float) position.y + v2r.y};
    Utility::Vector2f p3 = {(float) position.x + v3r.x, (float) position.y + v3r.y};

    // Round coordinates to nearest pixel
    p1.x = std::round(p1.x);
    p1.y = std::round(p1.y);
    p2.x = std::round(p2.x);
    p2.y = std::round(p2.y);
    p3.x = std::round(p3.x);
    p3.y = std::round(p3.y);

    // Clamp to screen bounds
    p1.x = fmaxf(0.0f, fminf(p1.x, (float) getGfx()->width() - 1.0f));
    p1.y = fmaxf(0.0f, fminf(p1.y, (float) getGfx()->height() - 1.0f));
    p2.x = fmaxf(0.0f, fminf(p2.x, (float) getGfx()->width() - 1.0f));
    p2.y = fmaxf(0.0f, fminf(p2.y, (float) getGfx()->height() - 1.0f));
    p3.x = fmaxf(0.0f, fminf(p3.x, (float) getGfx()->width() - 1.0f));
    p3.y = fmaxf(0.0f, fminf(p3.y, (float) getGfx()->height() - 1.0f));

    triangle.points[0] = p1;
    triangle.points[1] = p2;
    triangle.points[2] = p3;

    return triangle;
}

void Triangle::update(const Utility::Vector2i &pos) {
    if (m_fill) {
        getGfx()->fillTriangle((int16_t) m_triangle.points[0].x, (int16_t) m_triangle.points[0].y,
                               (int16_t) m_triangle.points[1].x, (int16_t) m_triangle.points[1].y,
                               (int16_t) m_triangle.points[2].x, (int16_t) m_triangle.points[2].y,
                               m_color);
    } else {
        getGfx()->drawTriangle((int16_t) m_triangle.points[0].x, (int16_t) m_triangle.points[0].y,
                               (int16_t) m_triangle.points[1].x, (int16_t) m_triangle.points[1].y,
                               (int16_t) m_triangle.points[2].x, (int16_t) m_triangle.points[2].y,
                               m_color);
    }

    Widget::update(pos);
}
