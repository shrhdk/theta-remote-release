/*
 * Copyright (c) 2016 Hideki Shiro
 */

#ifndef button_h
#define button_h

class Button {
public:
  Button(int port);
  bool isPressed();

private:
  const int port;
};

#endif
