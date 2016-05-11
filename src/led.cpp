/*
 * Copyright (c) 2015 Hideki Shiro
 */

#include "led.h"
#include "Arduino.h"

// public

LED::LED(int port) : port(port) {
    pinMode(port, OUTPUT);
    this->off();
}

void LED::on() {
    this->set(true);
}

void LED::off() {
    this->set(false);
}

void LED::toggle() {
    this->set(!this->get());
}

// private

bool LED::get() {
    return this->onOff;
}

void LED::set(bool onOff) {
    this->onOff = onOff;
    digitalWrite(port, onOff ? LOW : HIGH);
}
