/*
 * Copyright (c) 2015 Hideki Shiro
 */

#ifndef settings_h
#define settings_h

class SettingsClass {
public:
    char ssid[32];
    char password[32];
    void load();
    void flush();
private:
    static const char HEADER[];
    int pos;
    int verify();
    char read();
    void write(char c);
    void read(char *dst);
    void write(const char *src);
};

extern SettingsClass Settings;

#endif
