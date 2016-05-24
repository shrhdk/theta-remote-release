/*
 * Copyright (c) 2015 Hideki Shiro
 */

#include "theta_s.h"
#include "logger.h"
#include <strings.h>
#include <ESP8266WiFi.h>
#include <arduino.h>
#include <stdint.h>

// public

void ThetaSClass::connect(const char *ssid, const char *password) {
    WiFi.begin(ssid, password);
}

wl_status_t ThetaSClass::status() {
    return WiFi.status();
}

int ThetaSClass::startSession() {
    Logger.debug("ThetaS::startSession");

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

    return 0;
}

int ThetaSClass::updateSession() {
    Logger.debug("ThetaS::updateSession");

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

// private

const char ThetaSClass::HOST[] = "192.168.1.1";

const int ThetaSClass::PORT = 80;

int ThetaSClass::post(const char *path, const char *body, char *response) {
    Logger.debug("ThetaS::post");

    WiFiClient wifiClient;

    // Connect
    WiFiClient client;
    if (!client.connect(HOST, PORT)) {
        Logger.debug("connection failed");
        return 1;
    }

    // Construct Request

    String request;
    request += String("POST ") + path + " HTTP/1.1\r\n";
    request += String("Host: ") + HOST + ":" + PORT + "\r\n";
    request += "Content-Type: application/json;charset=utf-8\r\n";
    request += "Accept: application/json\r\n";
    request += String("Content-Length:") + strlen(body) + "\r\n";
    request += "\r\n";
    request += body;

    // Send Request

    Logger.debug("---- Request ----");

    Logger.debug(request.c_str());
    Logger.debug("");

    client.print(request);

    // Receive Response

    Logger.debug("---- Response ----");

    // Skip Header
    while(client.connected() || client.available()){
        String line = client.readStringUntil('\r');
        Logger.debug(line.c_str());
        if(line.equals("\n")) {
            Logger.debug("");
            break;
        }
    }

    // Read Body
    String responseString;
    while(client.available()) {
        responseString += client.readStringUntil('\r');
    }

    Logger.debug(responseString.c_str());

    strcpy(response, responseString.c_str());

    wifiClient.stop();

    return 0;
}

ThetaSClass ThetaS;
