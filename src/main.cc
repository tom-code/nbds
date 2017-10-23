

#include "net.h"
#include "nbds.h"


static void new_con(connection_t *con) {
  std::map<nbd_options_t, std::string> opts = {{nbd_options_t::SIZE, "10000000"}};
  nbds_new_con(con, opts);
}

int main()
{
  server("127.0.0.1", 10809, new_con);
}
