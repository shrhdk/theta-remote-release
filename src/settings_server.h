/*
 * Copyright (c) 2016 Hideki Shiro
 */

#ifndef settings_server_h
#define settings_server_h

#include <DNSServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>

class SettingsServerClass {
public:
  static void start();
  static void handle();

private:
  static ESP8266WebServer server;
  static ESP8266HTTPUpdateServer httpUpdater;
  static DNSServer dnsServer;
  static void handleRootGet();
  static void handleRootPost();
};

extern SettingsServerClass SettingsServer;

#endif
