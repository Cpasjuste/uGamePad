//
// Created by cpasjuste on 30/03/23.
//

#include "main.h"
#include "rectangle.h"
#include "ui.h"
#include "text.h"
#include "line.h"
#include "bitmap.h"
#include "bitmaps.h"
#include "gamepad_info.h"

using namespace uGamePad;

Rectangle *screen;

Ui::Ui() {
    screen = new Rectangle(0, 0, 128, 64);

    //auto cursor = new Line({2, 2}, 8, 90);
    //screen->add(cursor);

    auto gi = new GamePadInfo({64, 32}, {90, 60}, bmp_gamepad_90x60);
    gi->setOrigin(Widget::Origin::Center);
    screen->add(gi);
}

void Ui::loop() {
    screen->update(screen->getPosition());
}
