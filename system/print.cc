
#include "../include/LinkerDB.h"

/**
 * Prints all the Databases that inside database_table
 *
 * example: printDB
 */
void printDB(){
  printf("\n\nDB NAMES\n\n");
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
void printTable(table_t * table){
  printf("\n\n Table %s \n\n", table->table_name.c_str());
  for ( auto it = table->data.begin(); it != table->data.end(); ++it )
    printf("%s = %s\n", it->first.c_str(), it->second.str_data.c_str());
  printf("\n\n");
  return;
}

/**
 * Prints the structure from where specified down (can specifiy how far you want to go down)
 *
 * example: PRINTSTRUCTURE Rhythm
 * example: PRINTSTRUCTURE Rhythm 5
 */
void printStructure(table_t * table, int depth){

}
