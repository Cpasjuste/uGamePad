//
// Created by cpasjuste on 30/03/23.
//

#include "main.h"
#include "rectangle.h"
#include "gamepad_info.h"
#include "ui.h"
#include "text.h"
#include "line.h"
#include "bitmap.h"
#include "bitmaps.h"

using namespace uGamePad;

Ui::Ui() {
    p_screen = new Widget();
    p_screen->setSize(getPlatform()->getGfx()->getSize());

    //auto cursor = new Line({2, 2}, 8, 90);
    //p_screen->add(cursor);

    // gamepad info menu
    p_gamePadInfo = new GamePadInfo({64, 32}, {90, 60}, bmp_gamepad_90x60);
    p_gamePadInfo->setOrigin(Widget::Origin::Center);
    p_gamePadInfo->setVisibility(Widget::Visibility::Hidden);
    p_screen->add(p_gamePadInfo);

    // splash
    p_splash = new Bitmap({64, 32}, {90, 60}, bmp_gamepad_90x60);
    p_splash->setOrigin(Widget::Origin::Center);
    //p_splash->setVisibility(Widget::Visibility::Hidden);
    p_screen->add(p_splash);
    p_splash_text = new Text(64, 64, "uGamePad");
    p_splash_text->setOrigin(Widget::Origin::Bottom);
    //p_splash_text->setVisibility(Widget::Visibility::Hidden);
    p_screen->add(p_splash_text);

    // show splash
    showSplash();
}

void Ui::loop() {
    p_screen->update(p_screen->getPosition());
}

void Ui::showSplash() {
    getPlatform()->getGfx()->clear();
    p_splash->setVisibility(Widget::Visibility::Visible);
    Ui::loop();
    p_splash->setVisibility(Widget::Visibility::Hidden);
    getPlatform()->getGfx()->flip();
}

bool Ui::isActive() {
    return false;
}
