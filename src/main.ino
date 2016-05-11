/*
 * Copyright (c) 2015 Hideki Shiro
 */

#include "settings_server.h"
#include "logger.h"
#include "button.h"
#include "led.h"
#include "settings.h"
#include "theta_s.h"
#include <Ticker.h>

Button button(5);
LED led(4);

// THETA

#define UPDATE_PERIOD (160)

Ticker ticker;
bool needToUpdateSession = false;

void updateSessionFlag() {
    needToUpdateSession = true;
}

void connect(const char *ssid, const char *password) {
    led.off();

    ThetaS.connect(ssid, password);
    while(ThetaS.status() != WL_CONNECTED) {
        delay(300);
        led.toggle();
        Logger.debug(".");
    }

    startSession();

    led.on();
}

void startSession() {
    led.off();

    ThetaS.startSession();

    ticker.detach();
    needToUpdateSession = false;
    ticker.attach(UPDATE_PERIOD, updateSessionFlag);

    led.on();
}

void takePicture() {
    led.off();

    ThetaS.takePicture();
    startSession();

    delay(1000);

    led.on();
}

// Entry Point

void setup() {
    Logger.begin(LogLevel::DEBUG, 115200);
    Logger.debug("theta-remote-release");

    if(button.isPressed()) {
        led.on();
        SettingsServer.start();
        for(;;) {
            SettingsServer.handle();
        }
    }

    Settings.load();
}

void loop() {
    if(ThetaS.status() != WL_CONNECTED) {
        connect(Settings.ssid, Settings.password);
    } else if(needToUpdateSession) {
        startSession();
    } else if(button.isPressed()) {
        takePicture();
    }
}
