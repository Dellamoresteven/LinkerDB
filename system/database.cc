
#include "../include/LinkerDB.h"


int keyWordSearch(std::string key, int stage){
  /* checks for keywords in the command */
  if(key == "NEWDB"){
    return NEWDB;
  }
  if(key == "PUT"){
    return PUT;
  }
  if(key == "GET"){
    return GET;
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

    if(COMMAND_START_DEBUG) printf("Command\n");
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
        if(command == "printData"){
          printf("\n\nData\n\n");
          /* Goes through the database_names */
          // printf("DATA: %s\n", headTable.data->second.str_data);
          printf("\n\n\n");
          break;
        }
        int newStage = keyWordSearch(command, stage);

        if(newStage != stage) {stage = newStage; command = ""; continue;}

        /* A switch statment to know where in the command you should be */
        switch(stage){

          /**
           * Getting a value from the table specified
           *
           * example: Rhythm GET name
           * This will check Rhythm for a key value pair of 'name' and return its value, else return an empty object
           *
           * example: Rhythm->user GET name
           * This will check Rhythm for a link to 'user' then check for the key value pair 'name and reutrn its value'
           * else return an empty object
           */
          case GET: {
            if(GET_DEBUG) printf("GET: %s\n", command.c_str());
            std::vector<std::string> keys;
            std::string word;
            for(int j = 0; j < command.length(); j++) {
              if(command.at(j) == ','){
                keys.push_back(word);
                word = "";
              } else{
                word += command.at(j);
              }
            }
            if(word.length() != 0){ keys.push_back(word); }

            for(int j = 0; j < keys.size(); j++){
              if(GET_DEBUG) printf("check for: %s\n", keys.at(j).c_str());
              auto checksearch = headTable.data.find(keys.at(j));
              if(checksearch != headTable.data.end()){ /* found */
                printf("%s = %s\n", keys.at(j).c_str(), (checksearch->second.str_data).c_str());
              }else{ /* Did not find */
                printf("Did not find %s\n", keys.at(j).c_str());
              }
            }
            break;
          }

          /**
           * Inserting a value into the data of the table
           *
           * example: Rhythm PUT name=steve
           * This will go into Rhythm and PUT name = steve
           *
           * example: Rhythm PUT name,age,sex=steve,21,male
           * This will go into Rhythm DB and PUT name = steve, age = 21, sex = male
           */
          case PUT: {
            if(PUT_DEBUG) printf("PUT: %s\n", command.c_str());
            std::vector<std::string> keys;
            std::vector<std::string> values;
            bool isEqualsChar = true;
            std::string word;
            for(int j = 0; j < command.length(); j++) {
              if(command.at(j) == ',' || command.at(j) == '='){
                if(isEqualsChar){
                  keys.push_back(word);
                }else{
                  values.push_back(word);
                }
                if(command.at(j) == '=') {isEqualsChar = false;}
                word = "";
              }else{
                word += command.at(j);
              }
            }
            if(word.length() != 0){
              values.push_back(word);
            }

            for(int j = 0; j < keys.size(); j++){
              auto search = headTable.data.find(keys.at(j));
              if(search != headTable.data.end()){ /* already created */
                search->second.str_data = values.at(j);
              }else{ /* not created */
                data_t newEntry;
                newEntry.str_data = values.at(j);
                headTable.data.insert(std::make_pair(keys.at(j), newEntry));
              }
            }

            auto checksearch = headTable.data.find("steve");
            if(checksearch != headTable.data.end()){
              printf("found steve = %s\n", (checksearch->second.str_data).c_str());
            }
            break;
          }




          /**
           * This will deal with getting you to the correct data structer when a request comes in
           * it deals with the -> and what not.
           *
           * example: Rhythm->user->steven GET age
           * This will go to Rhythm, into User, into Steven, and return his age if its there.
           */
          case TABLE_LOOKUP: {
            std::string tableCommand;
            int check = true;
            /* Going through the string to check for '->' */
            for(int j = 0; j < command.length(); j++){
              /* Checking current char for '-' and then making sure thats not the last index and then checking for '>' */
              if((command.at(j) == '-') && (command.length() >= j+1) && (command.at(j+1) == '>')){
                j++;
                if(TABLE_LOOKUP_DEBUG ) printf("DBtableCommand: %s\n", tableCommand.c_str());
                if(check){
                  check = false;
                  auto search = database_table.database_names.find(tableCommand);
                  if(search != database_table.database_names.end()){
                    /* set the headtable of that DB */
                    headTable = search->second;
                    if(TABLE_LOOKUP_DEBUG) printf("Found DB\n");
                  }else{
                    if(TABLE_LOOKUP_DEBUG) printf("Could not find DB\n");
                    // return -1;
                  }
                }else{
                  /* seraching for the entry in the linked_table_names data struct */
                  auto tablesearch = headTable.linked_table_names.find(tableCommand);
                  /* Check to see if it exists */
                  if(tablesearch != headTable.linked_table_names.end()){ /* does exist */
                    if(TABLE_LOOKUP_DEBUG) printf("Found Table %s\n", tableCommand.c_str());
                    /* change the headTable to the new table */
                    headTable = tablesearch->second;
                  }else{ /* Does not exist */
                    if(TABLE_LOOKUP_DEBUG) printf("Could not find table %s\n", tableCommand.c_str());
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
            /* add the last command */
            if(tableCommand.length() == 0) {break;}
            /* If u are looking at the top of the database */
            if(check){
              check = false;
              auto search = database_table.database_names.find(tableCommand);
              if(search != database_table.database_names.end()){
                headTable = search->second;
                if(TABLE_LOOKUP_DEBUG) printf("Found in DB-end\n");
              }else{
                if(TABLE_LOOKUP_DEBUG) printf("Could not find in DB-end\n");
              }
              break;
            }
            auto datasearch = headTable.data.find(tableCommand);
            if(datasearch != headTable.data.end()){
              if(TABLE_LOOKUP_DEBUG) printf("Found in Data-end\n");
            }else{
              if(TABLE_LOOKUP_DEBUG) printf("Could not find in Data-end\n");
            }
            break;
          }







          /* Create the new Database */
          case NEWDB: {
            table_t bk;
            if(NEWDB_DEBUG) printf("NEW DB NAME:%s\n", command.c_str());
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
