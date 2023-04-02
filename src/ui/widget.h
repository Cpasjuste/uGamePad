//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_WIDGET_H
#define U_GAMEPAD_WIDGET_H

#include <cstdint>
#include <vector>

namespace uGamePad {
    class Widget {
    public:
        enum class Visibility : int {
            Visible, Hidden
        };

        Widget() = default;

        ~Widget();

        virtual void add(Widget *widget);

        virtual void remove(Widget *widget);

        virtual void setPosition(int16_t x, int16_t y);

        virtual void setPosition(const Utility::Vector2i &pos);

        virtual Utility::Vector2i getPosition();

        virtual void setSize(int16_t x, int16_t y);

        virtual void setSize(const Utility::Vector2i &size);

        virtual Utility::Vector2i getSize() { return m_size; };

        virtual bool isVisible() { return m_visibility == Visibility::Visible; };

        virtual void setVisibility(Visibility visibility) { m_visibility = visibility; };

        virtual Adafruit_GFX *getGfx();

        virtual void update(const Utility::Vector2i &pos);

    protected:
        Widget *p_parent = nullptr;
        std::vector<Widget *> p_child_list;
        Visibility m_visibility = Visibility::Visible;
        Utility::Vector2i m_position{};
        Utility::Vector2i m_size{};
    };
}

#endif //U_GAMEPAD_WIDGET_H
