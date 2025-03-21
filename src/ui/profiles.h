//
// Created by cpasjuste on 20/03/25.
//

#ifndef PROFILES_H
#define PROFILES_H

#define PROFILES_MAX 10

namespace uGamePad {
    class UiProfiles final : public Rectangle {
    public:
        UiProfiles();

        void setVisibility(Visibility visibility) override;

        void loop(const Utility::Vec2i &pos) override;

    private:
        void update();

        std::vector<MenuLine *> p_lines;
        uint8_t max_lines = 0;
        int option_index = 0;
        int highlight_index = 0;
    };
}

#endif //PROFILES_H
