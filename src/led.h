/*
 * Copyright (c) 2015 Hideki Shiro
 */

#ifndef led_h
#define led_h

#include <stdint.h>

class LED {
public:
    LED(int port);
    void on();
    void off();
    void toggle();
private:
    const int port;
    bool onOff;
    bool get();
    void set(bool onOff);
};

#endif
