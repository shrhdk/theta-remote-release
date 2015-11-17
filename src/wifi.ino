/*
 * Copyright (c) 2015 Hideki Shiro
 */

#include <ESP8266WiFi.h>

const char *ssid     = "";
const char *password = "";

bool isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

void connect() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (!isConnected()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
