//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"
#include "rectangle.h"
#include "ui.h"
#include "text.h"
#include "triangle.h"
#include "line.h"

using namespace uGamePad;

Rectangle *screen;

Ui::Ui() {
    screen = new Rectangle(0, 0, 128, 64);

    auto cursor = new Line({2, 2}, 8, 90);
    //screen->add(cursor);

    //for (uint16_t i = 0; i < UI_MAX_LINES; i++) {
    //    screen->add(new Text(5, (i * 9) + 6, "Hello uGamePad"));
    //}

    auto rect = new Rectangle(63, 31, 1, 1);
    screen->add(rect);

    auto text = new Text(64, 32, "HHHH");
    text->setOrigin(Widget::Origin::Center);
    screen->add(text);
}

void Ui::loop() {
    screen->update(screen->getPosition());
}
