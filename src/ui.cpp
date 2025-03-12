//
// Created by cpasjuste on 30/03/23.
//

#include "main.h"
#include "ui.h"
#include "text.h"
#include "bitmap.h"
#include "bitmaps.h"

using namespace uGamePad;

Ui::Ui() {
    p_screen = new Widget();
    p_screen->setSize(getPlatform()->getGfx()->getSize());

    // main menu
    p_menu = new Menu();
    p_menu->setVisibility(Widget::Visibility::Hidden);
    p_screen->add(p_menu);

    // gamepad info menu
    p_gamePadInfo = new GamePadSettings({64, 32}, {90, 60}, bmp_gamepad_90x60);
    p_gamePadInfo->setOrigin(Widget::Origin::Center);
    p_gamePadInfo->setVisibility(Widget::Visibility::Hidden);
    p_screen->add(p_gamePadInfo);

    p_systemInfo = new SystemInfo({0, 0}, p_screen->getSize());
    p_systemInfo->setVisibility(Widget::Visibility::Hidden);
    p_screen->add(p_systemInfo);

    // splash
    p_splash = new Bitmap({64, 28}, {64, 43}, bmp_gamepad_64x43);
    p_splash->setOrigin(Widget::Origin::Center);
    p_screen->add(p_splash);
    p_splashText = new Text(64, 62, "uGamePad @ " + getPlatform()->getPad()->getOutputMode()->name);
    p_splashText->setOrigin(Widget::Origin::Bottom);
    p_screen->add(p_splashText);

    // message box
    p_messageBox = new MessageBox(8, 8, 112, 48);
    p_screen->add(p_messageBox);

    // show splash screen
    show(Splash);
}

void Ui::show(const MenuWidget menuWidget) {
    //printf("Ui::show: %i\r\n", menuWidget);
    if (m_menuCurrent == menuWidget) return;

    if (menuWidget == Splash) {
        p_screen->setVisibility(Widget::Visibility::Visible);
        p_menu->setVisibility(Widget::Visibility::Hidden);
        p_gamePadInfo->setVisibility(Widget::Visibility::Hidden);
        p_systemInfo->setVisibility(Widget::Visibility::Hidden);
        p_splash->setVisibility(Widget::Visibility::Visible);
        p_splashText->setVisibility(Widget::Visibility::Visible);
        // draw
        flip();
        // disable screen updates when not needed (not in menu)
        p_screen->setVisibility(Widget::Visibility::Hidden);
        getPlatform()->getPad()->setRepeatDelay(0);
    } else if (menuWidget == MainMenu) {
        p_screen->setVisibility(Widget::Visibility::Visible);
        p_menu->setVisibility(Widget::Visibility::Visible);
        p_gamePadInfo->setVisibility(Widget::Visibility::Hidden);
        p_systemInfo->setVisibility(Widget::Visibility::Hidden);
        p_splash->setVisibility(Widget::Visibility::Hidden);
        p_splashText->setVisibility(Widget::Visibility::Hidden);
        getPlatform()->getPad()->setRepeatDelay(REPEAT_DELAY_DEFAULT);
    } else if (menuWidget == GamePadTest || menuWidget == Remap) {
        p_screen->setVisibility(Widget::Visibility::Visible);
        p_menu->setVisibility(Widget::Visibility::Hidden);
        p_splash->setVisibility(Widget::Visibility::Hidden);
        p_splashText->setVisibility(Widget::Visibility::Hidden);
        p_systemInfo->setVisibility(Widget::Visibility::Hidden);
        p_gamePadInfo->setVisibility(Widget::Visibility::Visible);
        getPlatform()->getPad()->setRepeatDelay(menuWidget == GamePadTest ? 0 : UINT16_MAX);
        p_gamePadInfo->setMode(menuWidget == GamePadTest ? GamePadSettings::Mode::Info : GamePadSettings::Mode::Remap);
    } else if (menuWidget == InfoMenu) {
        p_screen->setVisibility(Widget::Visibility::Visible);
        p_menu->setVisibility(Widget::Visibility::Hidden);
        p_gamePadInfo->setVisibility(Widget::Visibility::Hidden);
        p_systemInfo->setVisibility(Widget::Visibility::Visible);
        p_splash->setVisibility(Widget::Visibility::Hidden);
        p_splashText->setVisibility(Widget::Visibility::Hidden);
        getPlatform()->getPad()->setRepeatDelay(REPEAT_DELAY_DEFAULT);
    }

    m_menuCurrent = menuWidget;
}

void Ui::loop() {
    // check for menu combo keys
    if (!p_screen->isVisible()) {
        const auto buttons = getPlatform()->getPad()->getButtons();
        if (buttons & GamePad::Button::MENU) {
            p_menu->reset();
            show(MainMenu);
            // needed to clear hardware "menu/start" button
            getPlatform()->getPad()->flush();
        } else if (buttons & GamePad::Button::START && buttons & GamePad::Button::SELECT) {
            if (m_triggerMenuClock.getElapsedTime().asSeconds() > 1) {
                p_menu->reset();
                show(MainMenu);
            }
        } else if (!(buttons & GamePad::Button::DELAY)) {
            m_triggerMenuClock.restart();
        }
    } else {
        flip();
    }
}

void Ui::flip() {
    if (p_screen->isVisible()) {
        // draw the whole ui
        getPlatform()->getGfx()->clear();
        p_screen->loop(p_screen->getPosition());
        getPlatform()->getGfx()->flip();
    }
}
