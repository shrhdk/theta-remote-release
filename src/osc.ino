/*
 * Copyright (c) 2015 Hideki Shiro
 */

#include <string.h>
#include <ArduinoJson.h>

char buffer[256];
char sessionID[32];

void startSession() {
  // Construct Request
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& request = jsonBuffer.createObject();
  request["name"] = "camera.startSession";
  JsonObject& parameters = request.createNestedObject("parameters");
  request.printTo(buffer, sizeof(buffer));

  // Send Request
  String response = post("/osc/commands/execute", buffer);

  // Parse Response
  char responseChars[512];
  response.toCharArray(responseChars, sizeof(responseChars));
  JsonObject& root = jsonBuffer.parseObject(responseChars);

  strcpy(sessionID, root["results"]["sessionId"]);
}

void updateSession() {
  // Construct Request
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& request = jsonBuffer.createObject();
  request["name"] = "camera.updateSession";
  JsonObject& parameters = request.createNestedObject("parameters");
  parameters["sessionId"] = sessionID;
  request.printTo(buffer, sizeof(buffer));

  // Send Request
  String responseString = post("/osc/commands/execute", buffer);
}

void takePicture() {
  // Construct Request
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& request = jsonBuffer.createObject();
  request["name"] = "camera.takePicture";
  JsonObject& parameters = request.createNestedObject("parameters");
  parameters["sessionId"] = sessionID;
  request.printTo(buffer, sizeof(buffer));

  // Send Request
  String responseString = post("/osc/commands/execute", buffer);

  // Wait
  delay(3000);
}
