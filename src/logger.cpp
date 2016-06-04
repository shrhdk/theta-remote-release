/*
 * Copyright (c) 2016 Hideki Shiro
 */

#include "logger.h"
#include <arduino.h>

// public

void LoggerClass::begin(LogLevel level, int speed) {
  LoggerClass::level = level;
  Serial.begin(speed);
  delay(500);
}

void LoggerClass::trace(const char *message) {
  println(LogLevel::TRACE, message);
}

void LoggerClass::debug(const char *message) {
  println(LogLevel::DEBUG, message);
}

void LoggerClass::info(const char *message) {
  println(LogLevel::INFO, message);
}

void LoggerClass::warn(const char *message) {
  println(LogLevel::WARN, message);
}

void LoggerClass::error(const char *message) {
  println(LogLevel::ERROR, message);
}

// private

LogLevel LoggerClass::level = LogLevel::INFO;

void LoggerClass::println(LogLevel level, const char *message) {
  if (LoggerClass::level <= level) {
    Serial.println(message);
  }
}

LoggerClass Logger;
