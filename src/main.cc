

#include "net.h"
#include "nbds.h"


static void new_con(connection_t *con) {
  disk_t *disk = disk_new_ramdisk(1024*1024*100);
  //disk_t *disk = disk_new_file("/data/file.img");
  if (disk == nullptr) {
    con->close();
    return;
  }
  nbds_new_con(con, disk);
}

int main()
{
  server("127.0.0.1", 10809, new_con);
}
