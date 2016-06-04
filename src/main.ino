/*
 * Copyright (c) 2016 Hideki Shiro
 */

#include "settings_server.h"
#include "button.h"
#include "led.h"
#include "settings.h"
#include "theta_s.h"

Button button(5);
LED led(4);

// THETA

void connect(const char *ssid, const char *password) {
    ThetaS.connect(ssid, password);
    while(ThetaS.status() != WL_CONNECTED) {
        delay(300);
        led.toggle();
        Serial.print(".");
    }
}

// Entry Point

void setup() {
    Serial.begin(115200);
    Serial.println("theta-remote-release");

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
        led.off();
        connect(Settings.ssid, Settings.password);
        ThetaS.startSession();
        led.on();
    } else if(button.isPressed()) {
        led.off();
        ThetaS.shoot();
        led.on();
    }

    ThetaS.handle();
}
