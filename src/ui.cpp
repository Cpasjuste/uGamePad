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
    for (uint16_t i = 0; i < 6; i++) {
        //rect->add(new Text(5, (i * 9) + 6, "Hello uGamePad"));
    }

    auto line = new Line({2, 2}, 8, 90);
    screen->add(line);

    auto triangle = new Triangle({8, 8}, 8, -90);
    screen->add(triangle);
}

void Ui::loop() {
    screen->update(screen->getPosition());
}
