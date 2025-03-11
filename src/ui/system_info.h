//
// Created by cpasjuste on 09/03/25.
//

#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <particles.h>
#include <starfield.h>

namespace uGamePad {
    class SystemInfo final : public Rectangle {
    public:
        SystemInfo(const Utility::Vec2i &pos, const Utility::Vec2i &size);

        void setVisibility(Visibility visibility) override;

        void loop(const Utility::Vec2i &pos) override;

    private:
        ParticleBg *p_particles;
        StarFieldBg *p_starfield;
    };
}

#endif //SYSTEM_INFO_H
