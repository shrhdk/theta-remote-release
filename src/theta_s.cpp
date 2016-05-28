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

int ThetaSClass::startSession() {
    Logger.debug("ThetaS::startSession");

    disableUpdateSessionEvent();

    // Construct Request
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& request = jsonBuffer.createObject();
    request["name"] = "camera.startSession";
    request.printTo(buffer, sizeof(buffer));

    // Send Request
    if(post("/osc/commands/execute", buffer, buffer) != 0) {
        return 1;
    }

    // Parse Response
    JsonObject& response = jsonBuffer.parseObject(buffer);
    if(!response.success()) {
        Logger.error("Failed to parse response");
        return 1;
    }

    strcpy(sessionID, response["results"]["sessionId"]);

    enableUpdateSessionEvent();

    return 0;
}

int ThetaSClass::updateSession() {
    Logger.debug("ThetaS::updateSession");

    disableUpdateSessionEvent();

    // Construct Request
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& request = jsonBuffer.createObject();
    request["name"] = "camera.updateSession";
    JsonObject& parameters = request.createNestedObject("parameters");
    parameters["sessionId"] = sessionID;
    request.printTo(buffer, sizeof(buffer));

    // Send Request
    if(post("/osc/commands/execute", buffer, buffer) != 0) {
        return 1;
    }

    // Parse Response
    JsonObject& response = jsonBuffer.parseObject(buffer);
    if(!response.success()) {
        Logger.error("Failed to parse response");
        return 1;
    }

    strcpy(sessionID, response["results"]["sessionId"]);

    enableUpdateSessionEvent();

    return 0;
}

int ThetaSClass::takePicture() {
    Logger.debug("ThetaS::takePicture");

    // Construct Request
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& request = jsonBuffer.createObject();
    request["name"] = "camera.takePicture";
    JsonObject& parameters = request.createNestedObject("parameters");
    parameters["sessionId"] = sessionID;
    request.printTo(buffer, sizeof(buffer));

    // Send Request
    if(post("/osc/commands/execute", buffer, buffer) != 0) {
        return 1;
    }

    // Parse Response
    JsonObject& response = jsonBuffer.parseObject(buffer);

    if(!response.success()) {
        Logger.error("Failed to parse response");
        return 1;
    }

    return 0;
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

int ThetaSClass::post(const char *path, const char *body, char *response) {
    Logger.debug("ThetaS::post");

    HTTPClient http;
    http.begin(HOST, PORT, path);
    http.addHeader("Content-Type", "application/json;charset=utf-8");
    http.addHeader("Accept", "application/json");
    int statusCode = http.POST((uint8_t *)body, strlen(body));
    String result = http.getString();
    http.end();
    strcpy(response, result.c_str());
    return statusCode == 200 ? 0 : 1;
}

ThetaSClass ThetaS;
