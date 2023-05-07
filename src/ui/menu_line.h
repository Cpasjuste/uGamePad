//
// Created by cpasjuste on 07/05/23.
//

#ifndef U_GAMEPAD_MENU_LINE_H
#define U_GAMEPAD_MENU_LINE_H

#include "widget.h"

namespace uGamePad {
    class MenuLine : public Rectangle {
    public:
        explicit MenuLine(const Utility::Vec4i &rect);

        std::string getName();

        void setName(const std::string &name);

        std::string getValue();

        void setValue(const std::string &value);

        void setHighlighted(bool highlighted);

    private:
        Text *m_name;
        Text *m_value;
    };
} // uGamePad

#endif //U_GAMEPAD_MENU_LINE_H
