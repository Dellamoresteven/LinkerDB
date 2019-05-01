
#include "../include/LinkerDB.h"



int keyWordSearch(std::string key, int stage){
  /* checks for keywords in the command */
  if(key == "NEWDB"){
    return NEWDB;
  }
  else if(key == "PUT"){
    return PUT;
  }
  else if(key == "GET"){
    return GET;
  }else if(key == "PRINT"){
    return PRINT;
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

    if(COMMAND_START_DEBUG) printf("Command: \n");
    getline(std::cin, input);
    input += " ";
    std::string command;
    table_t * headTable;
    /* init new command to COMMAND_START */
    int stage = TABLE_LOOKUP;

    /* Runs through the whole command sent */
    for(int i = 0; i < input.length(); i++){
      /* checks for spaces */
      if(input.at(i) == ' '){
        if(COMMAND_START_DEBUG) printf("command = %s on stage %d\n", command.c_str(), stage);
        
        /* Prints the DB's (For testing Only) */
        if(command == "printDB"){
          printf("\n\nDB NAMES\n\n");
          /* Goes through the database_table map */
          for ( auto it = database_table.begin(); it != database_table.end(); ++it )
            std::cout << it->first << std::endl;
          printf("\n\n\n");
          break;
        }

        int newStage = keyWordSearch(command, stage);

        if(newStage != stage) {stage = newStage; command = ""; continue;}

        /* A switch statment to know where in the command you should be */
        switch(stage){

          case PRINT: {
            if(PRINT_DEBUG) printf("PRINT: %s\n", command.c_str());
            headTable = tableSearch(command);
            break;
          }

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
              auto checksearch = headTable->data.find(keys.at(j));
              if(checksearch != headTable->data.end()){ /* found */
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
              auto search = headTable->data.find(keys.at(j));
              if(search != headTable->data.end()){ /* already created */
                search->second.str_data = values.at(j);
              }else{ /* not created */
                data_t newEntry;
                newEntry.str_data = values.at(j);
                headTable->data.insert(std::make_pair(keys.at(j), newEntry));
              }
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
            headTable = tableSearch(command);
          }

          /* Create the new Database */
          case NEWDB: {
            table_t bk;
            bk.table_name = command;
            if(NEWDB_DEBUG) printf("NEW DB NAME:%s\n", command.c_str());
            database_table.insert(std::make_pair(command, bk));
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
