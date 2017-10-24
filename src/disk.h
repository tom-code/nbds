#ifndef _DISK_H
#define _DISK_H

#include <stdint.h>
#include <string>

class disk_t {
public:
  virtual void write(uint64_t offset, const unsigned char *data, int len) = 0;
  virtual void read(uint64_t offset, unsigned char *data, int len) = 0;
  virtual uint64_t size() = 0;
  virtual void resize(uint64_t s) = 0;
  virtual ~disk_t() {};
};
disk_t *disk_new_ramdisk(uint64_t size);
disk_t *disk_new_file(std::string path);


#endif