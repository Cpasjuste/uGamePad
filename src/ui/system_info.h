//
// Created by cpasjuste on 09/03/25.
//

#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <rectangle.h>

namespace uGamePad {
    class SystemInfo final : public Rectangle {
    public:
        SystemInfo(const Utility::Vec2i &pos, const Utility::Vec2i &size);

        void loop(const Utility::Vec2i &pos) override;
    };
}

#endif //SYSTEM_INFO_H
