/*
 * Copyright (c) 2016 Hideki Shiro
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
