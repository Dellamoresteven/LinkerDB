
#include "../include/LinkerDB.h"

void printDB(){
  printf("\n\nDB NAMES\n\n");
  /* Goes through the database_table map */
  for ( auto it = database_table.begin(); it != database_table.end(); ++it )
    std::cout << it->first << std::endl;
  printf("\n\n\n");
}

void printTable(table_t * table){
  printf("\n\n Table %s \n\n", table->table_name.c_str());
  for ( auto it = table->data.begin(); it != table->data.end(); ++it )
    printf("%s = %s\n", it->first.c_str(), it->second.str_data.c_str());
  printf("\n\n");
  return;
}
