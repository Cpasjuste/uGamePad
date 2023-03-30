//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"
#include "rectangle.h"
#include "ui.h"
#include "text.h"

using uGamePad::Ui;

uGamePad::Rectangle *rect;
uGamePad::Rectangle *rect2;
uGamePad::Text *text;

Ui::Ui() {
    rect = new uGamePad::Rectangle(0, 0, 128, 64);
    //rect2 = new uGamePad::Rectangle{2, 2, 128, 64};
    //rect2->add(new uGamePad::Rectangle(2, 2, 128, 64, true));
    //rect->add(rect2);

    text = new Text(2, 2, "Hello uGamePad");
    rect->add(text);
}

void uGamePad::Ui::update() {
    rect->update(rect->getPosition());
}
