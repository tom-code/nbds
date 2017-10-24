#include "disk.h"


#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>



class disk_file_t : public disk_t {
  int fd = -1;
  uint64_t siz = -1;
  unsigned char *mm = nullptr;
public:

  disk_file_t() {
  }

  bool init(std::string path) {
    fd = open(path.c_str(), O_RDWR);
    if (fd == -1) {
      printf("can't open %s\n", path.c_str());
      return false;
    }

    siz = lseek(fd, 0, SEEK_END);
    printf("size: %ld\n", siz);

    mm = (unsigned char*)mmap(0, siz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mm == MAP_FAILED) {
      close(fd);
      fd = -1;
      mm = nullptr;
      return false;
    }

    return true;
  }

  ~disk_file_t() {
    if (mm) munmap(mm, siz);
    if (fd > 0) close(fd);
  }

  void write(uint64_t offset, const unsigned char *bytes, int len) {
    memcpy(mm+offset, bytes, len);
  }

  void read(uint64_t offset, unsigned char *bytes, int len) {
    memcpy(bytes, mm+offset, len);
  }

  uint64_t size() {
    return siz;
  }

  void resize(uint64_t s) {
  }
};

disk_t *disk_new_file(std::string path) {
  if (access(path.c_str(), F_OK ) < 0) {
    printf("can't open file %s\n", path.c_str());
    return nullptr;
  }

  disk_file_t *d = new disk_file_t();
  if (!d->init(path)) {
    delete d;
    return nullptr;
  }

  return d;
}
