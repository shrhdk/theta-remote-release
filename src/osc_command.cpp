/*
 * Copyright (c) 2016 Hideki Shiro
 */

#include "osc_command.h"
#include <ESP8266HTTPClient.h>
#include <strings.h>

// public

OSCCommand::OSCCommand(const char *name) {
  request = aJson.createObject();
  aJson.addStringToObject(request, "name", name);
}

OSCCommand::~OSCCommand() {
  aJson.deleteItem(request);
  aJson.deleteItem(response);
}

int OSCCommand::execute() {
  char *body = aJson.print(request);

  HTTPClient http;
  http.begin("192.168.1.1", 80, "/osc/commands/execute");
  http.addHeader("Content-Type", "application/json;charset=utf-8");
  http.addHeader("Accept", "application/json");
  statusCode = http.POST((uint8_t *)body, strlen(body));
  String res = http.getString();
  http.end();

  strcpy(responseChars, res.c_str());
  response = aJson.parse(responseChars);

  if (400 <= statusCode && statusCode <= 599) {
    aJsonObject *error = aJson.getObjectItem(response, "error");
    code = aJson.getObjectItem(error, "code")->valuestring;
    message = aJson.getObjectItem(error, "message")->valuestring;
    return 1;
  }

  return 0;
}

// public Request

void OSCCommand::addParameter(const char *name, aJsonObject *value) {
  if (parameters == NULL) {
    parameters = aJson.createObject();
    aJson.addItemToObject(request, "parameters", parameters);
  }

  aJson.addItemToObject(parameters, name, value);
}

void OSCCommand::addParameter(const char *name, const char *value) {
  addParameter(name, aJson.createItem(value));
}

void OSCCommand::addParameter(const char *name, int value) {
  addParameter(name, aJson.createItem(value));
}

void OSCCommand::addParameter(const char *name, float value) {
  addParameter(name, aJson.createItem(value));
}

// public Response

aJsonObject *OSCCommand::getResultObject(const char *name) {
  if (response == NULL) {
    return NULL;
  }

  if (results == NULL) {
    results = aJson.getObjectItem(response, "results");
  }

  if (results == NULL) {
    return NULL;
  }

  aJson.getObjectItem(results, name);
}

aJsonObject *OSCCommand::getResultArray(const char *name) {
  return getResultObject(name);
}

char *OSCCommand::getResultString(const char *name) {
  aJsonObject *obj = getResultObject(name);
  return obj == NULL ? NULL : obj->valuestring;
}

bool OSCCommand::getResultBool(const char *name) {
  aJsonObject *obj = getResultObject(name);
  return obj == NULL ? false : obj->valuebool;
}

int OSCCommand::getResultNumber(const char *name) {
  aJsonObject *obj = getResultObject(name);
  return obj == NULL ? 0 : obj->valueint;
}

float OSCCommand::getResultFloat(const char *name) {
  aJsonObject *obj = getResultObject(name);
  return obj == NULL ? 0 : obj->valuefloat;
}
