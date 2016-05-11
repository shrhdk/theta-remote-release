/*
 * Copyright (c) 2015 Hideki Shiro
 */

#include "settings.h"
#include "logger.h"
#include <EEPROM.h>

// public

void SettingsClass::load() {
    this->pos = 0;

    EEPROM.begin(128);
    if(this->verify() != 0) {
        Logger.debug("No saved data.");
        strcpy(this->ssid, "");
        strcpy(this->password, "");
    } else {
        Logger.debug("Loading saved data.");
        this->read(this->ssid);
        this->read(this->password);
    }
    EEPROM.end();
}

void SettingsClass::flush() {
    this->pos = 0;

    EEPROM.begin(128);
    this->write(HEADER);
    this->write(this->ssid);
    this->write(this->password);
    EEPROM.end();
}

// private

const char SettingsClass::HEADER[] = "THETA";

int SettingsClass::verify() {
    for(int i = 0; i < strlen(HEADER) + 1; i++) {
        char c = this->read();
        if(HEADER[i] != c) {
            return 1;
        }
    }

    return 0;
}

char SettingsClass::read() {
    return EEPROM.read(this->pos++);
}

void SettingsClass::write(char c) {
    EEPROM.write(this->pos++, c);
}

void SettingsClass::read(char *dst) {
    for(int i = 0; ; i++) {
        dst[i] = this->read();

        if(dst[i] == '\0') {
            return;
        }
    }
}

void SettingsClass::write(const char *src) {
    for(int i = 0; ; i++) {
        this->write(src[i]);

        if(src[i] == '\0') {
            return;
        }
    }
}

SettingsClass Settings;
