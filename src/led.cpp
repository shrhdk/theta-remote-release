/*
 * Copyright (c) 2016 Hideki Shiro
 */

#include "led.h"
#include "Arduino.h"

// public

LED::LED(int port) : port(port) {
  pinMode(port, OUTPUT);
  off();
}

void LED::on() { set(true); }

void LED::off() { set(false); }

void LED::toggle() { set(!get()); }

// private

bool LED::get() { return onOff; }

void LED::set(bool onOff) {
  this->onOff = onOff;
  digitalWrite(port, onOff ? LOW : HIGH);
}
