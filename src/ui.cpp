//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"
#include "rectangle.h"
#include "ui.h"
#include "text.h"

using namespace uGamePad;

Rectangle *rect;

Ui::Ui() {
    rect = new Rectangle(0, 0, 128, 64);
    for (uint16_t i = 0; i < 6; i++) {
        rect->add(new Text(5, (i * 9) + 6, "Hello uGamePad"));
    }
}

void Ui::loop() {
    rect->update(rect->getPosition());
}
