
#include "../include/LinkerDB.h"


int stageChange(std::string key, int stage){
  /* checks for keywords in the command */
  if(key == "NEWDB"){
    return NEWDB;
  }
  if(key == "PUT"){
    return PUT;
  }
  /* return the current stage if keyword is not found */
  return stage;
}



int main(void){
  /* Init all system values (including reading from logs) */
  initDB();
  /* Full command */
  std::string input;

  while(true){

    if(DEBUGMSG) printf("Command\n");
    getline(std::cin, input);
    input += " ";
    std::string command;
    table_t headTable;
    /* init new command to COMMAND_START */
    int stage = TABLE_LOOKUP;

    /* Runs through the whole command sent */
    for(int i = 0; i < input.length(); i++){
      /* checks for spaces */
      if(input.at(i) == ' '){
        printf("command = %s on stage %d\n", command.c_str(), stage);
        /* Prints the DB's (For testing Only) */
        if(command == "printDB"){
          printf("\n\nDB NAMES\n\n");
          /* Goes through the database_names */
          for ( auto it = database_table.database_names.begin(); it != database_table.database_names.end(); ++it )
            std::cout << it->first << std::endl;
          printf("\n\n\n");
          break;
        }

        /* A switch statment to know where in the command you should be */
        switch(stage){


          case PUT: {
            printf("PUT: %s\n", command.c_str());
            
            break;
          }





          case TABLE_LOOKUP: {
            std::string tableCommand;
            int newStage = stageChange(command, stage);
            if(newStage != stage) {stage = newStage; break;}
            int check = true;
            /* Going through the string to check for '->' */
            for(int j = 0; j < command.length(); j++){
              /* Checking current char for '-' and then making sure thats not the last index and then checking for '>' */
              if((command.at(j) == '-') && (command.length() >= j+1) && (command.at(j+1) == '>')){
                j++;
                if(DEBUGMSG) printf("DBtableCommand: %s\n", tableCommand.c_str());
                if(check){
                  check = false;
                  auto search = database_table.database_names.find(tableCommand);
                  if(search != database_table.database_names.end()){
                    /* set the headtable of that DB */
                    headTable = search->second;
                    if(DEBUGMSG) printf("Found DB\n");
                  }else{
                    if(DEBUGMSG) printf("Could not find DB\n");
                    // return -1;
                  }
                }else{
                  /* seraching for the entry in the linked_table_names data struct */
                  auto tablesearch = headTable.linked_table_names.find(tableCommand);
                  /* Check to see if it exists */
                  if(tablesearch != headTable.linked_table_names.end()){ /* does exist */
                    if(DEBUGMSG) printf("Found Table %s\n", tableCommand.c_str());
                    /* change the headTable to the new table */
                    headTable = tablesearch->second;
                  }else{ /* Does not exist */
                    if(DEBUGMSG) printf("Could not find table %s\n", tableCommand.c_str());
                    break;
                  }
                  /* Reseeting the tableLookup string */

                }
                tableCommand = "";
              }else{
                /* Adding the next char to the tableLookup string */
                tableCommand += input.at(j);
              }
              /* tableCommand will not contain the last part of the table lookup */
            }
            if(tableCommand.length() == 0) {break;}
            if(check){
              check = false;
              auto search = database_table.database_names.find(tableCommand);
              if(search != database_table.database_names.end()){
                headTable = search->second;
                if(DEBUGMSG) printf("Found in DB-end\n");
              }else{
                if(DEBUGMSG) printf("Could not find in DB-end\n");
              }
              break;
            }
            auto datasearch = headTable.data.find(tableCommand);
            if(datasearch != headTable.data.end()){
              if(DEBUGMSG) printf("Found in Data-end\n");
            }else{
              if(DEBUGMSG) printf("Could not find in Data-end\n");
            }
            break;
          }







          /* Create the new Database */
          case NEWDB: {
            table_t bk;
            if(DEBUGMSG) printf("NEW DB NAME:%s\n", command.c_str());
            database_table.database_names.insert(std::make_pair(command, bk));
            break;
          }
        }
        /* Resets the command once we have delt with it */
        command = "";
      } else {
        /* Adds the letters to the command */
        command += input.at(i);
      }
    }
  }
}
