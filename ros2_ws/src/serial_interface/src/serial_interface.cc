#include "serial_interface/serial_interface.hh"

char* SerialInterface::cobs_encode(char* msg, int& sz) {
  int n = sz;
  int padding = sz / 254 + (sz % 254 > 0);

  sz = sz + padding + 1;
  printf("Current sz: %i\n", sz);
  char* e = new char[sz];
  e[0] = 0;

  int i = 0, ei = 1;
  // While i is still in msg
  while (i < n) {
    // Iterate through a 254 block (if possible)
    int next_stop = std::min(i + 254, n);
    printf("Cur: %i Next stop: %i\n", i, next_stop);
    for (; i < next_stop; ++i, ++ei)
      e[ei] = msg[i];
    // Set the next byte to 0
    printf("Current ei: %i\n", ei);
    e[ei++] = 0;
  }
  // e[sz - 1] = 0;

  printf("Pre replacing zero byte stream: ");
  print_byte_stream(e, sz);

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

void SerialInterface::send(char* s, int sz) {
  printf("Original byte stream: ");
  print_byte_stream(s, sz);
  char *e = cobs_encode(s, sz);
#ifdef RASP5
  serialPuts(fd, e);
#else
  printf("COBS-encoded byte stream: ");
  print_byte_stream(e, sz);
#endif

  delete e;
}

void SerialInterface::print_byte_stream (char* msg, int sz) {
  for (int i = 0; i < sz; ++i) {
    printf("%d ", msg[i]);
  }
  printf("\n");
}

SerialInterface::SerialInterface(std::string device) {
  #ifdef RASP5
  fd = serialOpen(device.c_str(), 9600);
  #endif
  printf("Using device: %s\n", device.c_str());
}
SerialInterface::~SerialInterface() {
  #ifdef RASP5
  serialClose(fd);
  #endif
}
