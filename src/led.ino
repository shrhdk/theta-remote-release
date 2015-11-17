/*
 * Copyright (c) 2015 Hideki Shiro
 */

#define PIN_LED (4)

bool setUpLED() {
  pinMode(PIN_LED, OUTPUT);
}

void setLED(boolean onOff) {
  digitalWrite(PIN_LED, onOff ? LOW : HIGH);
}
