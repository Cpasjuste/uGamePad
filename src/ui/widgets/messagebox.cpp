//
// Created by cpasjuste on 12/03/25.
//

#include "main.h"
#include "messagebox.h"

using namespace uGamePad;

MessageBox::MessageBox(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
    : Rectangle(x, y, w, h, true, 0, Utility::Color::Black) {
    // outline
    MessageBox::add(new Rectangle(0, 0, w, h));

    p_text = new Text(w / 2, h / 3, "Hello World!");
    p_text->setOrigin(Origin::Center);
    MessageBox::add(p_text);

    p_buttonLeft = new Text(w * 0.25f, h - 6, "YES");
    p_buttonLeft->setOrigin(Origin::Bottom);
    MessageBox::add(p_buttonLeft);

    p_buttonRight = new Text(w * 0.75f, h - 6, "NO", Utility::Color::Black);
    p_buttonRight->setDrawBackground(true);
    p_buttonRight->setOrigin(Origin::Bottom);
    MessageBox::add(p_buttonRight);

    MessageBox::setVisibility(Visibility::Hidden);
}

void MessageBox::show(const std::string &text, const std::string &leftButton, const std::string &rightButton,
                      const std::function<void(const Text *)> &callback) {
    const Text *selectedText = nullptr;

    p_text->setString(text);

    if (!leftButton.empty() && !rightButton.empty()) {
        p_buttonLeft->setString(leftButton);
        p_buttonLeft->setPosition(getSize().x * 0.25f, getSize().y - 6);
        p_buttonLeft->setVisibility(Visibility::Visible);
        p_buttonRight->setString(rightButton);
        p_buttonRight->setPosition(getSize().x * 0.75f, getSize().y - 6);
        p_buttonRight->setVisibility(Visibility::Visible);
        // reset "buttons" state
        p_buttonLeft->setColor(Utility::Color::White);
        p_buttonLeft->setDrawBackground(false);
        p_buttonRight->setColor(Utility::Color::Black);
        p_buttonRight->setDrawBackground(true);
    } else {
        p_buttonLeft->setString("OK");
        p_buttonLeft->setPosition(getSize().x / 2, getSize().y - 6);
        p_buttonLeft->setVisibility(Visibility::Visible);
        p_buttonRight->setVisibility(Visibility::Hidden);
        // reset "buttons" state
        p_buttonLeft->setColor(Utility::Color::Black);
        p_buttonLeft->setDrawBackground(true);
    }

    setVisibility(Visibility::Visible);

    while (true) {
        getPlatform()->getHid()->loop();
        getPlatform()->getPad()->loop();
        getPlatform()->getUi()->loop();

        const auto buttons = getPlatform()->getPad()->getButtons();
        if (buttons & GamePad::Button::B2) {
            break;
        }

        if (!leftButton.empty() && !rightButton.empty()) {
            if (buttons & GamePad::Button::LEFT || buttons & GamePad::Button::RIGHT
                || buttons & GamePad::Button::UP || buttons & GamePad::Button::DOWN) {
                if (p_buttonLeft->getColor() == Utility::Color::White) {
                    p_buttonLeft->setColor(Utility::Color::Black);
                    p_buttonLeft->setDrawBackground(true);
                    p_buttonRight->setColor(Utility::Color::White);
                    p_buttonRight->setDrawBackground(false);
                } else {
                    p_buttonLeft->setColor(Utility::Color::White);
                    p_buttonLeft->setDrawBackground(false);
                    p_buttonRight->setColor(Utility::Color::Black);
                    p_buttonRight->setDrawBackground(true);
                }
            }
        }

        if (buttons & GamePad::Button::B1 || buttons & GamePad::Button::MENU) {
            selectedText = p_buttonLeft->getColor() == Utility::Color::Black ? p_buttonLeft : p_buttonRight;
            // needed to clear hardware "menu/start" button
            if (buttons & GamePad::Button::MENU) getPlatform()->getPad()->flush();
            break;
        }
    }

    setVisibility(Visibility::Hidden);
    if (callback) callback(selectedText);
}

void MessageBox::loop(const Utility::Vec2i &pos) {
    Rectangle::loop(pos);
}
