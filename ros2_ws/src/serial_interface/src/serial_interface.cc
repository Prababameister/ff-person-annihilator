#include "serial_interface/serial_interface.hh"

char* SerialInterface::cobs_encode(char* msg) {
  int n = strlen(msg);
  int padding = n / 254 + (n % 254 > 0);

  char* e = new char[n + padding + 1];
  e[0] = 0;

  int i = 0, ei = 1;
  // While i is still in msg
  while (i < n) {
    // Iterate through a 254 block (if possible)
    int next_stop = std::min(i + 254, n);
    for (; i < next_stop; ++i, ++ei)
      e[ei] = msg[i];
    // Set the next byte to 0
    e[ei++] = 0;
  }

  // Now set zeroes point to the next zero
  int last = --ei;
  --ei;
  for (; ei >= 0; --ei) {
    if (e[ei] == 0) {
      e[ei] = last - ei;
      last = ei;
    }
  }

  return e;
}

void SerialInterface::send(char* s) {
  char *e = cobs_encode(s);
  serialPuts(fd, e);
  delete e;
}

SerialInterface::SerialInterface(std::string device) {
  fd = serialOpen(device.c_str(), 9600);
}
SerialInterface::~SerialInterface() {
  serialClose(fd);
}
