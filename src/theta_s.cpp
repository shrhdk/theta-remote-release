/*
 * Copyright (c) 2015 Hideki Shiro
 */

#include "theta_s.h"
#include "logger.h"
#include <strings.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <arduino.h>
#include <stdint.h>
#include <Ticker.h>

// file scope

Ticker ticker;

const int UPDATE_SESSION_PERIOD = 160;

bool needToUpdateSession = false;

void updateSessionFlag() {
    needToUpdateSession = true;
}

void enableUpdateSessionEvent() {
    needToUpdateSession = false;
    ticker.attach(UPDATE_SESSION_PERIOD, updateSessionFlag);
}

void disableUpdateSessionEvent() {
    ticker.detach();
}

// public

void ThetaSClass::connect(const char *ssid, const char *password) {
    WiFi.begin(ssid, password);
}

wl_status_t ThetaSClass::status() {
    return WiFi.status();
}

int ThetaSClass::getCaptureStatus() {
    Logger.debug("ThetaS::getCaptureStatus");

    // Construct Request
    strcpy(buffer, "");

    // Send Request
    post("/osc/state", buffer, buffer);

    // Parse Response
    jsonBuffer = StaticJsonBuffer<512>();
    JsonObject& response = jsonBuffer.parseObject(buffer);
    if(!response.success()) {
        Logger.error("Failed to parse response");
        return 1;
    }

    strcpy(captureStatus, response["state"]["_captureStatus"]);

    return 0;
}

int ThetaSClass::startSession() {
    Logger.debug("ThetaS::startSession");

    disableUpdateSessionEvent();

    OSCCommand cmd("camera.startSession");

    cmd.execute();

    if(cmd.statusCode != 200) {
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

    if(cmd.statusCode != 200) {
        return 1;
    }

    strcpy(sessionID, cmd.getResultString("sessionId"));

    enableUpdateSessionEvent();

    return 0;
}

int ThetaSClass::getCaptureMode() {
    Logger.debug("ThetaS::getCaptureMode");

    OSCCommand cmd("camera.getOptions");
    cmd.addParameter("sessionId", sessionID);
    aJsonObject *optionNames = aJson.createArray();
    aJson.addItemToArray(optionNames, aJson.createItem("captureMode"));
    cmd.addParameter("optionNames", optionNames);

    cmd.execute();

    if(cmd.statusCode != 200) {
        return 1;
    }

    aJsonObject *options = cmd.getResultArray("options");
    strcpy(captureMode, aJson.getObjectItem(options, "captureMode")->valuestring);

    return 0;
}

int ThetaSClass::takePicture() {
    Logger.debug("ThetaS::takePicture");

    OSCCommand cmd("camera.takePicture");
    cmd.addParameter("sessionId", sessionID);

    cmd.execute();

    if(cmd.statusCode != 200) {
        return 1;
    }

    return 0;
}

int ThetaSClass::startCapture() {
    Logger.debug("ThetaS::startCapture");

    OSCCommand cmd("camera._startCapture");
    cmd.addParameter("sessionId", sessionID);

    cmd.execute();

    if(cmd.statusCode != 200) {
        return 1;
    }

    return 0;
}

int ThetaSClass::stopCapture() {
    Logger.debug("ThetaS::stopCapture");

    OSCCommand cmd("camera._stopCapture");
    cmd.addParameter("sessionId", sessionID);

    cmd.execute();

    if(cmd.statusCode != 200) {
        return 1;
    }

    return 0;
}

int ThetaSClass::shoot() {
    if(getCaptureStatus()) {
        return 1;
    }
    Logger.debug(captureStatus);

    if(getCaptureMode()) {
        return 1;
    }
    Logger.debug(captureMode);

    if(strcmp(captureMode, "image") == 0) {
        if(strcmp(captureStatus, "idle") == 0) {
            if(takePicture()) {
                return 1;
            }
        }
    } else if(strcmp(captureMode, "_video") == 0) {
        if(strcmp(captureStatus, "idle") == 0) {
            if(startCapture()) {
                return 1;
            }
        } else if (strcmp(captureStatus, "shooting") == 0) {
            if(stopCapture()) {
                return 1;
            }
        }
    }
}

int ThetaSClass::handle() {
    if(needToUpdateSession) {
        return ThetaS.updateSession();
    }

    return 0;
}

// private

const char ThetaSClass::HOST[] = "192.168.1.1";

const int ThetaSClass::PORT = 80;

void ThetaSClass::post(const char *path, const char *body, char *response) {
    Logger.debug("ThetaS::post");

    HTTPClient http;
    http.setReuse(true);
    http.begin(HOST, PORT, path);
    http.addHeader("Content-Type", "application/json;charset=utf-8");
    http.addHeader("Accept", "application/json");
    statusCode = http.POST((uint8_t *)body, strlen(body));
    String result = http.getString();
    http.end();
    strcpy(response, result.c_str());
}

ThetaSClass ThetaS;
