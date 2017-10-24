#include "disk.h"

#include <vector>
#include <string.h>


class disk_ram_t : public disk_t {
  std::vector<unsigned char> data;
public:
  void write(uint64_t offset, const unsigned char *bytes, int len) {
    memcpy(data.data()+offset, bytes, len);
  }
  void read(uint64_t offset, unsigned char *bytes, int len) {
    memcpy(bytes, data.data()+offset, len);
  }
  uint64_t size() {
    return data.size();
  }
  void resize(uint64_t s) {
    data.resize(s);
  }
};

disk_t *disk_new_ramdisk(uint64_t size) {
  disk_ram_t *d = new disk_ram_t();
  d->resize(size);
  return d;
}