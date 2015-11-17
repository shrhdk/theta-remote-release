/*
 * Copyright (c) 2015 Hideki Shiro
 */

void setup() {
  Serial.begin(115200);
  setUpLED();
  setUpButton();
  delay(100);
  Serial.println("theta-remote-release");
}

void loop() {
  if(!isConnected()) {
    connect();
  } else if(isPressed()) {
    startSession();
    takePicture();
    while(isPressed());
  }
}
