#pragma once

#include <wiringSerial.h>

#include <string>

class SerialInterface {
  public:
    SerialInterface(char* device);
    ~SerialInterface();

    void send(char* s, int sz);
  private:
    int fd;

    char* cobs_encode(char* msg);
};
