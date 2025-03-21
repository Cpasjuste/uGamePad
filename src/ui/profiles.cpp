//
// Created by cpasjuste on 20/03/25.
//

#include "main.h"
#include "profiles.h"

using namespace uGamePad;

UiProfiles::UiProfiles() : Rectangle({0, 0}, getPlatform()->getGfx()->getSize()) {
    const uint8_t menu_height = UiProfiles::getSize().y - 4;
    uint8_t line_height = 7 + 2; // font height + margin
    max_lines = menu_height / line_height;
    if (max_lines * line_height < menu_height) {
        line_height = menu_height / max_lines;
    }

    // menu lines
    for (uint8_t i = 0; i < max_lines; i++) {
        auto y = static_cast<int16_t>((line_height * i) + 2);
        auto w = static_cast<int16_t>(UiProfiles::getSize().x - 2);
        auto line = new MenuLine({1, y, w, line_height});
        p_lines.push_back(line);
        UiProfiles::add(line);
    }
}

void UiProfiles::setVisibility(Visibility visibility) {
    if (visibility == Visibility::Visible) update();
    Rectangle::setVisibility(visibility);
}

void UiProfiles::update() {
    const auto device = getPlatform()->getPad()->getDevice();
    if (!device) return;

    const auto slot = getPlatform()->getConfig()->getSlot(device->vid, device->pid);

    // update lines
    for (uint8_t i = 0; i < max_lines; i++) {
        const auto index = option_index + i;
        if (index >= PROFILES_MAX) {
            p_lines[i]->setVisibility(Visibility::Hidden);
        } else {
            if (index == slot) {
                p_lines[i]->setName("SLOT " + std::to_string(index) + " (*)");
            } else {
                p_lines[i]->setName("SLOT " + std::to_string(index));
            }
            p_lines[i]->setValue("EDIT");
            p_lines[i]->setVisibility(Visibility::Visible);

            // handle highlight
            p_lines[i]->setHighlighted(i == highlight_index);
        }
    }
}

void UiProfiles::loop(const Utility::Vec2i &pos) {
    const auto buttons = getPlatform()->getPad()->getButtons();
    if (buttons & GamePad::Button::UP) {
        const int index = option_index + highlight_index;
        const int middle = max_lines / 2;
        if (highlight_index <= middle && index - middle > 0) {
            option_index--;
        } else {
            highlight_index--;
        }
        if (highlight_index < 0) {
            highlight_index = PROFILES_MAX < max_lines ? PROFILES_MAX - 1 : max_lines - 1;
            option_index = PROFILES_MAX - 1 - highlight_index;
        }
        update();
    } else if (buttons & GamePad::Button::DOWN) {
        const int index = option_index + highlight_index;
        const int middle = max_lines / 2;
        if (highlight_index >= middle && index + middle < PROFILES_MAX) {
            option_index++;
        } else {
            highlight_index++;
        }
        if (highlight_index >= max_lines || option_index + highlight_index >= PROFILES_MAX) {
            option_index = 0;
            highlight_index = 0;
        }
        update();
    }

    Rectangle::loop(pos);
}
