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
    if(post("/osc/state", buffer, buffer) != 0) {
        return 1;
    }

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

    // Construct Request
    jsonBuffer = StaticJsonBuffer<512>();
    JsonObject& request = jsonBuffer.createObject();
    request["name"] = "camera.startSession";
    request.printTo(buffer, sizeof(buffer));

    // Send Request
    if(post("/osc/commands/execute", buffer, buffer) != 0) {
        return 1;
    }

    // Parse Response
    jsonBuffer = StaticJsonBuffer<512>();
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
    jsonBuffer = StaticJsonBuffer<512>();
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
    jsonBuffer = StaticJsonBuffer<512>();
    JsonObject& response = jsonBuffer.parseObject(buffer);
    if(!response.success()) {
        Logger.error("Failed to parse response");
        return 1;
    }

    strcpy(sessionID, response["results"]["sessionId"]);

    enableUpdateSessionEvent();

    return 0;
}

int ThetaSClass::getCaptureMode() {
    Logger.debug("ThetaS::getCaptureMode");

    // Construct Request
    jsonBuffer = StaticJsonBuffer<512>();
    JsonObject& request = jsonBuffer.createObject();
    request["name"] = "camera.getOptions";
    JsonObject& parameters = request.createNestedObject("parameters");
    parameters["sessionId"] = sessionID;
    JsonArray& optionNames = parameters.createNestedArray("optionNames");
    optionNames.add("captureMode");
    request.printTo(buffer, sizeof(buffer));

    Logger.debug(buffer);
    // Send Request
    if(post("/osc/commands/execute", buffer, buffer) != 0) {
        Logger.error("Failed to send request");
        return 1;
    }

    // Parse Response
    jsonBuffer = StaticJsonBuffer<512>();
    JsonObject& response = jsonBuffer.parseObject(buffer);
    if(!response.success()) {
        Logger.error("Failed to parse response");
        return 1;
    }

    strcpy(captureMode, response["results"]["options"]["captureMode"]);

    return 0;
}

int ThetaSClass::takePicture() {
    Logger.debug("ThetaS::takePicture");

    // Construct Request
    jsonBuffer = StaticJsonBuffer<512>();
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
    jsonBuffer = StaticJsonBuffer<512>();
    JsonObject& response = jsonBuffer.parseObject(buffer);

    if(!response.success()) {
        Logger.error("Failed to parse response");
        return 1;
    }

    return 0;
}

int ThetaSClass::startCapture() {
    Logger.debug("ThetaS::startCapture");

    // Construct Request
    jsonBuffer = StaticJsonBuffer<512>();
    JsonObject& request = jsonBuffer.createObject();
    request["name"] = "camera._startCapture";
    JsonObject& parameters = request.createNestedObject("parameters");
    parameters["sessionId"] = sessionID;
    request.printTo(buffer, sizeof(buffer));

    // Send Request
    if(post("/osc/commands/execute", buffer, buffer) != 0) {
        return 1;
    }

    // Parse Response
    jsonBuffer = StaticJsonBuffer<512>();
    JsonObject& response = jsonBuffer.parseObject(buffer);

    if(!response.success()) {
        Logger.error("Failed to parse response");
        return 1;
    }

    return 0;
}

int ThetaSClass::stopCapture() {
    Logger.debug("ThetaS::stopCapture");

    // Construct Request
    jsonBuffer = StaticJsonBuffer<512>();
    JsonObject& request = jsonBuffer.createObject();
    request["name"] = "camera._stopCapture";
    JsonObject& parameters = request.createNestedObject("parameters");
    parameters["sessionId"] = sessionID;
    request.printTo(buffer, sizeof(buffer));

    // Send Request
    if(post("/osc/commands/execute", buffer, buffer) != 0) {
        return 1;
    }

    // Parse Response
    jsonBuffer = StaticJsonBuffer<512>();
    JsonObject& response = jsonBuffer.parseObject(buffer);

    if(!response.success()) {
        Logger.error("Failed to parse response");
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

    if(statusCode != 200) {
        Logger.debug((String("Receive error response: ") + statusCode + " " + result).c_str());
        return 1;
    }

    return 0;
}

ThetaSClass ThetaS;
