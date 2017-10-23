
#include "net.h"
#include <string.h>
#include <vector>
#include <stdint.h>

#include "buffers.h"



const static unsigned char magic_magic[]={0x4e, 0x42, 0x44, 0x4d, 0x41, 0x47, 0x49, 0x43};
const static unsigned char magic_opt[]  ={0x49, 0x48, 0x41, 0x56, 0x45, 0x4f, 0x50, 0x54};
const static unsigned char magic_flg[]  ={0x00, 0x00};
const static uint32_t magic_reply = 0x67446698;

const static uint16_t nbd_cmd_read  = 0;
const static uint16_t nbd_cmd_write = 1;
const static uint16_t nbd_cmd_disc  = 2;

class nbds_t {
  connection_t *con;
  in_buffer_t in_buffer;

  enum class state_wait_for_t {FLAGS, EXPORT, TRANS, NONE};
  state_wait_for_t wait_for = state_wait_for_t::FLAGS;

  std::vector<unsigned char> disk;

public:
  nbds_t() {
    disk.resize(1024*1024*10);
  }

  void set_connection(connection_t *c) {
    con = c;
    con->set_read_cb(std::bind(&nbds_t::newdata, this, std::placeholders::_1, std::placeholders::_2));
    con->set_release_cb([this] {delete this;});

    //send initial welcome
    con->write(magic_magic, sizeof(magic_magic));
    con->write(magic_opt,   sizeof(magic_opt));
    con->write(magic_flg,   sizeof(magic_flg));
  }

private:
  bool handle_option() {
    if (in_buffer.size() <16) return false;
    uint64_t magic = in_buffer.get_uint64();
    uint32_t opt = in_buffer.get_uint32();
    uint32_t opt_size = in_buffer.get_uint32();
    printf("%lx %x %x\n", magic, opt, opt_size);
    in_buffer.sync();
      
    out_buffer_t out;
    out.add_uint64(disk.size());
    out.add_uint16(0);
    out.add_zero(124);
    con->write(out.data(), out.size());
    wait_for = state_wait_for_t::TRANS;
    return true;
  }

  bool handle_command() {
    if (in_buffer.size() < 28) return false;
    /*uint32_t magic =*/ in_buffer.get_uint32();
    /*uint16_t flags =*/ in_buffer.get_uint16();
    uint16_t type = in_buffer.get_uint16();
    uint64_t handle = in_buffer.get_uint64();
    uint64_t offset = in_buffer.get_uint64();
    uint32_t len = in_buffer.get_uint32();
      
    printf("%d %ld %ld %d\n", type, handle, offset, len);
    if (type == nbd_cmd_read) {
      in_buffer.sync();
      out_buffer_t out;
      out.add_uint32(magic_reply);
      out.add_uint32(0);
      out.add_uint64(handle);
      out.add_bytes(disk.data()+offset, len);
      con->write(out.data(), out.size());
    }
    else if (type == nbd_cmd_write) {
      if (in_buffer.remain() < len) {
        printf("wait for %d have %ld\n", len, in_buffer.remain());
        in_buffer.reset();
        return false;
      }
      memcpy(disk.data()+offset, in_buffer.get_bytes(), len);
      in_buffer.skip(len);
      in_buffer.sync();

      out_buffer_t out;
      out.add_uint32(magic_reply);
      out.add_uint32(0);
      out.add_uint64(handle);
      con->write(out.data(), out.size());
    }
    else if (type == nbd_cmd_disc) {
      printf("disc\n");
      con->close();
      return false;
    } else {
      printf("unknown command type %d\n", type);
    }
    return true;
  }

  void newdata(const unsigned char *data, int len) {
    in_buffer.add(data, len);
    while (in_buffer.size() > 0) {
      if (wait_for == state_wait_for_t::FLAGS) {
        if (in_buffer.size() <4) return;
        wait_for = state_wait_for_t::EXPORT;
        in_buffer.consume(4);
      }
      if (wait_for == state_wait_for_t::EXPORT) {
        if (!handle_option()) return;
      }
      if (wait_for == state_wait_for_t::TRANS) {
        if (!handle_command()) return;
      }
    }
  }

};

void nbds_new_con(connection_t *con) {
  nbds_t *n = new nbds_t();
  n->set_connection(con);
}
