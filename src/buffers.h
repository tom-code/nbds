#include <vector>
#include <stdint.h>
#include <arpa/inet.h>

//simple decoding and encoding buffers

#ifndef htobe64
  #define htobe64(x) (((uint64_t) htonl((uint32_t) ((x) >> 32))) | (((uint64_t) htonl((uint32_t) x)) << 32))
#endif

struct in_buffer_t {
  std::vector<unsigned char> buffer;

  size_t size() {
    return buffer.size();
  }
  void add(const unsigned char *data, int len) {
    buffer.insert(buffer.end(), data, data+len);
  }
  void consume(size_t len) {
    buffer.erase(buffer.begin(), buffer.begin()+len);
  }

  int ptr = 0;
  uint32_t get_uint32() {
    uint32_t ret = ntohl(*((uint32_t*)(buffer.data()+ptr)));
    ptr += 4;
    return ret;
  }
  uint32_t get_uint16() {
    uint16_t ret = ntohs(*((uint16_t*)(buffer.data()+ptr)));
    ptr += 2;
    return ret;
  }
  uint64_t get_uint64() {
    uint64_t ret = htobe64(*((uint64_t*)(buffer.data()+ptr)));
    ptr += 8;
    return ret;
  }
  unsigned char *get_bytes() {
    return buffer.data()+ptr;
  }
  void skip(int n) {
    ptr += n;
  }
  void sync() {
    consume(ptr);
    ptr = 0;
  }
  void reset() {
    ptr = 0;
  }
  size_t remain() {
    return buffer.size()-ptr;
  }
};

struct out_buffer_t {
  std::vector<unsigned char> buf;

  void add_uint16(uint16_t d) {
    int ptr = buf.size();
    buf.resize(ptr+2);
    *((uint16_t*)(buf.data()+ptr)) = htons(d);
  }

  void add_uint32(uint32_t d) {
    int ptr = buf.size();
    buf.resize(ptr+4);
    *((uint32_t*)(buf.data()+ptr)) = htonl(d);
  }

  void add_uint64(uint64_t d) {
    int ptr = buf.size();
    buf.resize(ptr+8);
    *((uint64_t*)(buf.data()+ptr)) = htobe64(d);
  }

  void add_zero(int n) {
    int ptr = buf.size();
    buf.resize(ptr+n);
    memset(buf.data()+ptr, 0, n);
  }

  void add_bytes(unsigned char *b, int n) {
    buf.insert(buf.end(), b, b+n);
  }
  unsigned char *reserve(int n) {
    buf.reserve(buf.size()+n);
    unsigned char *out = buf.data()+buf.size();
    buf.resize(buf.size()+n);
    return out;
  }

  unsigned char *data() {
    return buf.data();
  }
  size_t size() {
    return buf.size();
  }
};
