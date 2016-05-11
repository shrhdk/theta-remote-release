/*
 * Copyright (c) 2015 Hideki Shiro
 */

#ifndef _THETA_S_H_
#define _THETA_S_H_

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

class ThetaSClass {
public:
    void connect(const char *ssid, const char *password);
    wl_status_t status();
    int startSession();
    int takePicture();
private:
    static const char HOST[];
    static const int PORT;
    char buffer[512];
    char sessionID[16];
    int post(const char *path, const char *body, char *response);
};

extern ThetaSClass ThetaS;

#endif
