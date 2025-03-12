//
// Created by cpasjuste on 12/03/25.
//

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

namespace uGamePad {
    class MessageBox final : public Rectangle {
    public:
        MessageBox(int16_t x, int16_t y, int16_t w, int16_t h);

        void loop(const Utility::Vec2i &pos) override;

        void show(const std::string &text, const std::string &leftButton, const std::string &rightButton,
                  const std::function<void(const Text *)> &callback);

        Text *getText() { return p_text; }

        Text *getButtonLeft() { return p_buttonLeft; }

        Text *getButtonRight() { return p_buttonRight; }

    private:
        Text *p_text;
        Text *p_buttonLeft;
        Text *p_buttonRight;
    };
}

#endif //MESSAGEBOX_H
