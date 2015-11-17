/*
 * Copyright (c) 2015 Hideki Shiro
 */

#define PIN_BUTTON (0)

bool setUpButton() {
  pinMode(PIN_BUTTON, INPUT);
}

bool isPressed() {
  return digitalRead(PIN_BUTTON) == 0;
}
