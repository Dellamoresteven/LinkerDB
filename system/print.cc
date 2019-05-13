
#include "../include/LinkerDB.h"

/**
 * Prints all the Databases that inside database_table
 *
 * example: printDB
 */
void printDB(){
  printf("\n\nDatabase Names\n\n");
  /* Goes through the database_table map */
  for ( auto it = database_table.begin(); it != database_table.end(); ++it )
    std::cout << it->first << std::endl;
  printf("\n\n\n");
}

/**
 * Prints the contents of the table specified
 *
 * example: print hello->user->steven
 */
std::string printTable(table_t * table){
  std::string resp;
  resp = "\n{\n";
  for ( auto it = table->data.begin(); it != table->data.end(); ++it )
    resp += (it->first + ": " + it->second.str_data + ",\n");
  resp += "}\n";
  return resp;
}

/**
 * Prints the structure from where specified down (can specifiy how far you want to go down)
 *
 * example: PRINTSTRUCTURE Rhythm
 * example: PRINTSTRUCTURE Rhythm 5
 */
void printStructure(table_t * table, int depth){

}

/**
 * Prints the links that connect to the specified link
 *
 * example: DESC Rhythm
 * example: DESC Rhythm->user
 */
std::string describeTable(table_t * table){
  std::string resp = "\n{\n";
  for ( auto it = table->linked_table_names.begin(); it != table->linked_table_names.end(); ++it )
    resp += it->first + ",\n";
  resp += "}\n";
  return resp;
}
