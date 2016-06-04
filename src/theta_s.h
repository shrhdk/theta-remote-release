/*
 * Copyright (c) 2016 Hideki Shiro
 */

#ifndef _THETA_S_H_
#define _THETA_S_H_

#include <ESP8266WiFi.h>

class ThetaSClass {
public:
  void connect(const char *ssid, const char *password);
  wl_status_t status();
  int startSession();
  int updateSession();
  int takePicture();
  int startCapture();
  int stopCapture();
  int shoot();
  int handle();

private:
  char sessionID[64];
  int executeSimpleCommand(const char *name);
};

extern ThetaSClass ThetaS;

#endif
