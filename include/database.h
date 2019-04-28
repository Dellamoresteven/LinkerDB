
#define MAXTABLES 50
#define DEBUGMSG 1

enum command_state_table {
  COMMAND_START,
  NEWDB
};

struct database_t {
  std::unordered_map<std::string, table_t> database_names;
};

extern struct database_t database_table;
