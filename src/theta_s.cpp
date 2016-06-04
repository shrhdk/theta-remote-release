/*
 * Copyright (c) 2016 Hideki Shiro
 */

#include "theta_s.h"
#include "logger.h"
#include "osc_command.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <arduino.h>
#include <stdint.h>
#include <strings.h>

// file scope

Ticker ticker;

const int UPDATE_SESSION_PERIOD = 160;

bool needToUpdateSession = false;

void updateSessionFlag() { needToUpdateSession = true; }

void enableUpdateSessionEvent() {
  needToUpdateSession = false;
  ticker.attach(UPDATE_SESSION_PERIOD, updateSessionFlag);
}

void disableUpdateSessionEvent() { ticker.detach(); }

// public

void ThetaSClass::connect(const char *ssid, const char *password) {
  WiFi.begin(ssid, password);
}

wl_status_t ThetaSClass::status() { return WiFi.status(); }

int ThetaSClass::startSession() {
  Logger.debug("ThetaS::startSession");

  disableUpdateSessionEvent();

  OSCCommand cmd("camera.startSession");

  cmd.execute();

  if (cmd.statusCode != 200) {
    return 1;
  }

  strcpy(sessionID, cmd.getResultString("sessionId"));

  enableUpdateSessionEvent();

  return 0;
}

int ThetaSClass::updateSession() {
  Logger.debug("ThetaS::updateSession");

  disableUpdateSessionEvent();

  // Construct Request
  OSCCommand cmd("camera.updateSession");
  cmd.addParameter("sessionId", sessionID);

  cmd.execute();

  if (cmd.statusCode != 200) {
    return 1;
  }

  strcpy(sessionID, cmd.getResultString("sessionId"));

  enableUpdateSessionEvent();

  return 0;
}

int ThetaSClass::takePicture() {
  return executeSimpleCommand("camera.takePicture");
}

int ThetaSClass::startCapture() {
  return executeSimpleCommand("camera._startCapture");
}

int ThetaSClass::stopCapture() {
  return executeSimpleCommand("camera._stopCapture");
}

int ThetaSClass::shoot() {
  int result = takePicture();

  if (result != 2) {
    return result;
  }

  result = startCapture();

  if (result != 2) {
    return result;
  }

  return stopCapture();
}

int ThetaSClass::handle() {
  if (needToUpdateSession) {
    return ThetaS.updateSession();
  }

  return 0;
}

// private

int ThetaSClass::executeSimpleCommand(const char *name) {
  Logger.debug(name);

  OSCCommand cmd(name);
  cmd.addParameter("sessionId", sessionID);

  cmd.execute();

  if (strcmp(cmd.code, "disabledCommand") == 0) {
    return 2;
  }

  if (cmd.statusCode != 200) {
    return 1;
  }

  return 0;
}

ThetaSClass ThetaS;
