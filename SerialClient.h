#ifndef __SERIAL_CLIENT_H
#define __SERIAL_CLIENT_H
	
#include "Client.h"
#include "IPAddress.h"

class SerialClient : public Client {

public:
  SerialClient();
  SerialClient(int baud);

  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port);
  virtual size_t write(uint8_t b);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int available();
  virtual int read();
  virtual int read(uint8_t *buf, size_t size);
  virtual int peek();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool();
};

#endif
