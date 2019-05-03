
#define MAXTABLES 50

/* -----------------------DEBUG STATMENTS---------------------------------------- */
#define COMMAND_START_DEBUG 1
#define NEWDB_DEBUG 1
#define TABLE_LOOKUP_DEBUG 1
#define PUT_DEBUG 1
#define GET_DEBUG 1
#define PRINT_DEBUG 1
#define INIT_DEBUG 1
#define RESPONSE_DEBUG 1
/* ------------------------------------------------------------------------------ */

enum command_state_table {
  COMMAND_START,
  NEWDB,
  TABLE_LOOKUP,
  PUT,
  GET,
  PRINT,
  NEWLINK,
  PRINTSTRUC
};

extern std::unordered_map<std::string, table_t> database_table;
