//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"
#include "line.h"

using namespace uGamePad;

Line::Line(const Utility::Vector2i &pos, int16_t size, float rotation) : Widget() {
    Widget::setPosition(pos);
    Widget::setSize(size, size);
    m_rotation = rotation;
    m_line = calculateLine({(float) pos.x, (float) pos.y}, m_rotation, (float) size);
}

void Line::update(const Utility::Vector2i &pos) {
    if (isVisible()) {
        //if (m_rotation == 0 || m_rotation == 180 || m_rotation == 360) {
        //    getGfx()->drawFastHLine(m_line.start.x, m_line.start.y, m_size.x, Utility::Color::White);
        //} else if (m_rotation == 90 || m_rotation == 270) {
        //    getGfx()->drawFastVLine(m_line.start.x, m_line.start.y, m_size.x, Utility::Color::White);
        //} else {
        getGfx()->drawLine((int16_t) m_line.start.x, (int16_t) m_line.start.y,
                           (int16_t) m_line.end.x, (int16_t) m_line.end.y, Utility::Color::White);
        //}
    }

    Widget::update(pos);
}

Utility::Line Line::calculateLine(Utility::Vector2f position, float rotation, float length) {
    Utility::Line line{};
    Utility::Vector2f start = {0.0f, 0.0f};
    Utility::Vector2f end = {length, 0.0f};

    // Create rotation matrix
    float radians = rotation * (float) M_PI / 180.0f;
    Utility::Matrix2D rotationMatrix = Utility::rotationMatrix(radians);

    // Create transformation matrix
    Utility::Matrix2D transformationMatrix{};
    transformationMatrix.m11 = rotationMatrix.m11 * length;
    transformationMatrix.m12 = rotationMatrix.m12 * length;
    transformationMatrix.m21 = rotationMatrix.m21 * length;
    transformationMatrix.m22 = rotationMatrix.m22 * length;
    transformationMatrix.m11 += position.x;
    transformationMatrix.m12 += position.y;
    transformationMatrix.m21 += position.x;
    transformationMatrix.m22 += position.y;

    // Transform points
    start = Utility::transformPoint(start, transformationMatrix);
    end = Utility::transformPoint(end, transformationMatrix);

    // Round coordinates to nearest pixel
    start.x = std::round(start.x);
    start.y = std::round(start.y);
    end.x = std::round(end.x);
    end.y = std::round(end.y);

    // Limit coordinates to grid
    start.x = std::max(std::min(start.x, 127.0f), 0.0f);
    start.y = std::max(std::min(start.y, 63.0f), 0.0f);
    end.x = std::max(std::min(end.x, 127.0f), 0.0f);
    end.y = std::max(std::min(end.y, 63.0f), 0.0f);

    line.start = start;
    line.end = end;

    return line;
}
