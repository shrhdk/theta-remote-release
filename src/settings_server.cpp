/*
 * Copyright (c) 2016 Hideki Shiro
 */

#include "settings_server.h"
#include "settings.h"
#include <string.h>

// public

void SettingsServerClass::start() {
  const int DNS_PORT = 53;
  const IPAddress IP(192, 168, 1, 1);
  const IPAddress MASK(255, 255, 255, 0);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IP, IP, MASK);
  WiFi.softAP("theta-remote-release");

  dnsServer.setTTL(60);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, "*", IP);

  httpUpdater.setup(&server);
  server.on("/", HTTP_GET, handleRootGet);
  server.on("/", HTTP_POST, handleRootPost);

  server.begin();
}

void SettingsServerClass::handle() {
  dnsServer.processNextRequest();
  server.handleClient();
}

// private

ESP8266WebServer SettingsServerClass::server(80);

ESP8266HTTPUpdateServer SettingsServerClass::httpUpdater;

DNSServer SettingsServerClass::dnsServer;

void SettingsServerClass::handleRootGet() {
  String html = "<html>";
  html += "<body>";
  html += "<h1>WiFi Settings</h1>";
  html += "<form method='post'>";
  html += "<input type='text' name='ssid' placeholder='ssid'><br>";
  html += "<input type='text' name='password' placeholder='password'><br>";
  html += "<input type='submit'><br>";
  html += "</form>";
  html += "</body>";
  html += "</html>";
  server.send(200, "text/html", html);
}

void SettingsServerClass::handleRootPost() {
  strcpy(Settings.ssid, server.arg("ssid").c_str());
  strcpy(Settings.password, server.arg("password").c_str());
  Settings.flush();

  String html = "";
  html += "<h1>Success!</h1>";
  html += "<p>" + String(Settings.ssid) + "</p>";
  html += "<p>" + String(Settings.password) + "</p>";

  server.send(200, "text/html", html);
}

SettingsServerClass SettingsServer;
