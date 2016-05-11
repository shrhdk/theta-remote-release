/*
 * Copyright (c) 2015 Hideki Shiro
 */

#include "button.h"
#include "arduino.h"

Button::Button(int port) : port(port) {
    pinMode(port, INPUT);
}

bool Button::isPressed() {
    return digitalRead(this->port) == LOW;
}
