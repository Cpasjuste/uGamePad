//
// Created by cpasjuste on 09/03/25.
//

#include "main.h"
#include "system_info.h"

using namespace uGamePad;

SystemInfo::SystemInfo(const Utility::Vec2i &pos, const Utility::Vec2i &size) : Rectangle(pos, size) {
    p_particles = new ParticleBg(0, 0, 128, 64);
    p_starfield = new StarFieldBg(0, 0, 128, 64);
    SystemInfo::add(p_particles);
    SystemInfo::add(p_starfield);

    SystemInfo::add(new Text(4, 4, "uG. Software: v" APP_VERSION));
    SystemInfo::add(new Text(4, 16, "uG. Hardware: " HW_VERSION));
    SystemInfo::add(new Text(4, 28, "uG. Build: " BUILD_DATE));

    const auto t = new Text(size.x / 2, size.y - 4, "2025 @ Cpasjuste");
    t->setOrigin(Origin::Bottom);
    SystemInfo::add(t);
}

void SystemInfo::setVisibility(Visibility visibility) {
    if (visibility == Visibility::Visible) {
        // random bg
        p_particles->setVisibility(rand() % 2 ? Visibility::Visible : Visibility::Hidden);
        p_starfield->setVisibility(p_particles->isVisible() ? Visibility::Hidden : Visibility::Visible);
    }

    Rectangle::setVisibility(visibility);
}

void SystemInfo::loop(const Utility::Vec2i &pos) {
    const auto buttons = getPlatform()->getPad()->getButtons();
    if (buttons & GamePad::Button::MENU || buttons & GamePad::Button::B2) {
        getPlatform()->getUi()->show(Ui::MenuWidget::MainMenu);
        // needed to clear hardware "menu/start" button
        if (buttons & GamePad::Button::MENU) getPlatform()->getPad()->flush();
    }

    Rectangle::loop(pos);
}
