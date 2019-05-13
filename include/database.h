
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

/************************************************************
* The databaseHandler(std::string) is the main of LinkerDB  *
* (see server.cc for actual main). This is where all the    *
* commands are sent and handled. Firstly, it takes the      *
* string inputed and sends it to seperateCommand.cc where   *
* the string is split into tokens. From there it will start *
* going through all the tokens and handling them one by one.*
* If you are looking for desciption of commands you can     *
* look inside this file for examples                        *
************************************************************/
std::string databaseHandler(std::string input);

extern std::unordered_map<std::string, table_t *> database_table;
