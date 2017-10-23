
#include <string>
#include <map>

enum class nbd_options_t { SIZE };
void nbds_new_con(connection_t *con, std::map<nbd_options_t, std::string> opts);
