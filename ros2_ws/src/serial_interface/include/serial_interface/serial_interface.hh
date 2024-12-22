#pragma once

#include <cstring>
#include <algorithm>
#include <string>

#ifdef RASP5
#include <wiringSerial.h>
#endif

class SerialInterface {
  public:
    SerialInterface(std::string device);
    ~SerialInterface();

    void send(char* s, int sz);
  private:
    int fd;

    void print_byte_stream(char* msg, int sz);
    char* cobs_encode(char* msg, int& sz);
};
