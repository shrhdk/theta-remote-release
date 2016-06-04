/*
 * Copyright (c) 2016 Hideki Shiro
 */

#include "settings.h"
#include "logger.h"
#include <EEPROM.h>

// public

void SettingsClass::load() {
    pos = 0;

    EEPROM.begin(128);
    if(verify() != 0) {
        Logger.debug("No saved data.");
        strcpy(ssid, "");
        strcpy(password, "");
    } else {
        Logger.debug("Loading saved data.");
        read(ssid);
        read(password);
    }
    EEPROM.end();
}

void SettingsClass::flush() {
    pos = 0;

    EEPROM.begin(128);
    write(HEADER);
    write(ssid);
    write(password);
    EEPROM.end();
}

// private

const char SettingsClass::HEADER[] = "THETA";

int SettingsClass::verify() {
    for(int i = 0; i < strlen(HEADER) + 1; i++) {
        char c = read();
        if(HEADER[i] != c) {
            return 1;
        }
    }

    return 0;
}

char SettingsClass::read() {
    return EEPROM.read(pos++);
}

void SettingsClass::write(char c) {
    EEPROM.write(pos++, c);
}

void SettingsClass::read(char *dst) {
    for(int i = 0; ; i++) {
        dst[i] = read();

        if(dst[i] == '\0') {
            return;
        }
    }
}

void SettingsClass::write(const char *src) {
    for(int i = 0; ; i++) {
        write(src[i]);

        if(src[i] == '\0') {
            return;
        }
    }
}

SettingsClass Settings;
