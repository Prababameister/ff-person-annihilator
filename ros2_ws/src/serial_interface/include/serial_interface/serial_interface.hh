#pragma once

#include <cstring>
#include <algorithm>
#include <string>

#include <wiringSerial.h>

class SerialInterface {
  public:
    SerialInterface(std::string device);
    ~SerialInterface();

    void send(char* s);
  private:
    int fd;

    char* cobs_encode(char* msg);
};
