
#include "../include/LinkerDB.h"




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
    /* init new command to COMMAND_START */
    int stage = COMMAND_START;

    /* Runs through the whole command sent */
    for(int i = 0; i < input.length(); i++){
      /* checks for spaces */
      if(input.at(i) == ' '){
        /* Prints the DB's (For testing Only) */
        if(command == "printDB"){
          printf("\n\nDB NAMES\n\n");
          /* Goes through the database_names */
          for ( auto it = database_table.database_names.begin(); it != database_table.database_names.end(); ++it )
            std::cout << it->first << std::endl;
          printf("\n\n\n");
        }
        /* A switch statment to know where in the command you should be */
        switch(stage){
          /* First time the command comes in */
          case COMMAND_START:
            /* If this is a new NEWDB */
            if(command == "NEWDB"){
              stage = NEWDB;
            }else{
              /* If this is an exisiting DB look for it in the database_table map */
              auto search = database_table.database_names.find(command);
              if(DEBUGMSG) printf("Checking DBname: %s Size = %lu\n", command.c_str(), database_table.database_names.size());
              /* If the DB is real, else if the DB is not real */
              if(search != database_table.database_names.end()){
                if(DEBUGMSG) printf("Found DB\n");
              }else{
                if(DEBUGMSG) printf("Could not find DB\n");
                // return -1;
              }
            }
            break;
          /* Create the new Database */
          case NEWDB:
            table_t bk;
            if(DEBUGMSG) printf("NEW DB NAME:%s\n", command.c_str());
            database_table.database_names.insert(std::make_pair(command, bk));
            break;
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
