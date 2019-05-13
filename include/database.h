
#define MAXTABLES 50

/* -----------------------DEBUG STATMENTS---------------------------------------- */
#define COMMAND_START_DEBUG 0
#define NEWDB_DEBUG 0
#define TABLE_LOOKUP_DEBUG 0
#define PUT_DEBUG 0
#define GET_DEBUG 0
#define PRINT_DEBUG 0
#define INIT_DEBUG 0
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
  PRINTSTRUC,
  DESC
};

std::string databaseHandler(std::string input);

extern std::unordered_map<std::string, table_t *> database_table;
