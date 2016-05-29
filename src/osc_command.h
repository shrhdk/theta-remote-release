/*
 * Copyright (c) 2016 Hideki Shiro
 */

#ifndef _OSC_COMMAND_H_
#define _OSC_COMMAND_H_

#include <aJSON.h>

class OSCCommand {
public:
    OSCCommand(const char *name);
    virtual ~OSCCommand();
    int execute();
    // Request
    void addParameter(const char *name, aJsonObject *value);
    void addParameter(const char *name, const char *value);
    void addParameter(const char *name, bool value);
    void addParameter(const char *name, int value);
    void addParameter(const char *name, float value);
    // Response
    int statusCode;
    char responseChars[512];
    aJsonObject *getResultObject(const char *name);
    aJsonObject *getResultArray(const char *name);
    char *getResultString(const char *name);
    bool getResultBool(const char *name);
    int getResultNumber(const char *name);
    float getResultFloat(const char *name);
    // Error
    char *code;
    char *message;
private:
    // Request
    aJsonObject *request = NULL;
    aJsonObject *parameters = NULL;
    // Response
    aJsonObject *response = NULL;
    aJsonObject *results = NULL;
};

#endif
