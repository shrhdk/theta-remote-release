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
    int getCaptureStatus();
    int startSession();
    int updateSession();
    int getCaptureMode();
    int startCapture();
    int stopCapture();
    int takePicture();
    int shoot();
    int handle();
private:
    static const char HOST[];
    static const int PORT;
    int statusCode;
    char buffer[512];
    StaticJsonBuffer<512> jsonBuffer;
    char captureStatus[64];
    char sessionID[64];
    char captureMode[64];
    void post(const char *path, const char *body, char *response);
};

extern ThetaSClass ThetaS;

#endif
