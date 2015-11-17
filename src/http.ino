/*
 * Copyright (c) 2015 Hideki Shiro
 */

#include <string.h>
#include <ESP8266WiFi.h>

const char *host = "192.168.1.1";
const int port = 80;

struct Response {
  String header;
  String body;
};

String post(const char *path, const char *body) {
  // Connect

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return "";
  }

  // Construct Request

  String request;
  request += String("POST ") + path + " HTTP/1.1\r\n";
  request += String("Host: ") + host + ":" + port + "\r\n";
  request += "Content-Type: application/json;charset=utf-8\r\n";
  request += "Accept: application/json\r\n";
  request += String("Content-Length:") + strlen(body) + "\r\n";
  request += "\r\n";
  request += body;

  // Send Request

  Serial.println(request);
  Serial.println("");

  client.print(request);

  // Receive Response

  // Skip Header
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    if(line.equals("\n")) {
      break;
    }
  }

  // Read Body
  String response;
  while(client.available()) {
    response += client.readStringUntil('\r');
  }

  Serial.println("Response: " + response);
  Serial.println("");

  return response;
}
