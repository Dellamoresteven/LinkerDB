
#include "../include/LinkerDB.h"

table_t * tableSearch(std::string path){
  std::string tableCommand;
  table_t * returnTable;
  /* Checks to see if i should be reading for database or tables */
  int check = true;
  /* Im not actually sure if this is needed, I just wanted a lambda function */
  auto a = []() -> table_t* { table_t * ret = new table_t; ret->table_name = "-1"; return ret; };
  /* Going through the string to check for '->' */
  for(int j = 0; j < path.length(); j++) {
    /* Checking current char for '-' and then making sure thats not the last index and then checking for '>' */
    if((path.at(j) == '-') && (path.length() >= j+1) && (path.at(j+1) == '>')){
      j++;
      if(TABLE_LOOKUP_DEBUG ) printf("DBtableCommand: %s\n", tableCommand.c_str());
      if(check){
        check = false;
        auto search = database_table.find(tableCommand);
        if(search != database_table.end()){
          /* set the returnTable of that DB */
          returnTable = &(search->second);
          if(TABLE_LOOKUP_DEBUG) printf("Found DB\n");
        }else{
          if(TABLE_LOOKUP_DEBUG) printf("Could not find DB\n");
          return a();
        }
      }else{
        /* seraching for the entry in the linked_table_names data struct */
        auto tablesearch = returnTable->linked_table_names.find(tableCommand);
        /* Check to see if it exists */
        if(tablesearch != returnTable->linked_table_names.end()){ /* does exist */
          if(TABLE_LOOKUP_DEBUG) printf("Found Table %s\n", tableCommand.c_str());
          /* change the returnTable to the new table */
          returnTable = (tablesearch->second);
        }else{ /* Does not exist */
          if(TABLE_LOOKUP_DEBUG) printf("Could not find table %s\n", tableCommand.c_str());
          return a();
        }
        /* Reseeting the tableLookup string */

      }
      tableCommand = "";
    }else{
      /* Adding the next char to the tableLookup string */
      tableCommand += path.at(j);
    }
    /* tableCommand will not contain the last part of the table lookup */
  }

  /* add the last command */
  if(tableCommand.length() == 0) {return returnTable;}
  /* If u are looking at the top of the database */
  if(check){
    check = false;
    auto search = database_table.find(tableCommand);
    if(search != database_table.end()){
      returnTable = &(search->second);
      if(TABLE_LOOKUP_DEBUG) printf("Found in DB-end\n");
    }else{
      if(TABLE_LOOKUP_DEBUG) printf("Could not find in DB-end\n");
      return a();
    }
  } else{
    /* seraching for the entry in the linked_table_names data struct */
    auto tablesearch = returnTable->linked_table_names.find(tableCommand);
    /* Check to see if it exists */
    if(tablesearch != returnTable->linked_table_names.end()){ /* does exist */
      if(TABLE_LOOKUP_DEBUG) printf("Found Table %s\n", tableCommand.c_str());
      /* change the returnTable to the new table */
      returnTable = (tablesearch->second);
    }else{ /* Does not exist */
      if(TABLE_LOOKUP_DEBUG) printf("Could not find table %s\n", tableCommand.c_str());
      return a();
    }
    /* Reseeting the tableLookup string */
  }
  return returnTable;
}
