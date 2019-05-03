
#include "../include/LinkerDB.h"



int keyWordSearch(std::string key, int stage){
  /* checks for keywords in the command */
  if(key == "NEWDB"){
    return NEWDB;
  }
  else if(key == "PUT"){
    return PUT;
  }else if(key == "GET"){
    return GET;
  }else if(key == "PRINT"){
    return PRINT;
  }else if(key == "NEWLINK"){
    return NEWLINK;
  } else if(key == "printStruc"){
    return PRINTSTRUC;
  }
  /* return the current stage if keyword is not found */
  return stage;
}



int main(void){

  /* Init all system values (including reading from logs) */
  initDB();

  /* Full command */
  std::string input;
  std::vector<std::string> toks;
  while(true){
    if(COMMAND_START_DEBUG) printf("Command: \n");
    getline(std::cin, input);
    toks.clear();
    seperateCommand(input, toks);
    for(auto i : toks){
      printf("HERED: %s\n", i.c_str());
    }
    table_t * headTable;

    /* init new command to COMMAND_START */
    int stage = TABLE_LOOKUP;

    /* Runs through all the tokens */
    for(int i = 0; i < toks.size(); i++){
      /* checks for spaces */
      if(COMMAND_START_DEBUG) printf("Command = %s on stage %d\n", toks.at(i).c_str(), stage);

      /* Prints the DB's (For testing Only) */
      if(toks.at(i) == "printDB"){
        printDB();
        break;
      }

      int newStage = keyWordSearch(toks.at(i), stage);

      if(newStage != stage) {stage = newStage; toks.at(i) = ""; continue;}

      /* A switch statment to know where in the command you should be */
      switch(stage){

        case NEWLINK: {
          table_t t;
          t.table_name = toks.at(i);
          headTable->linked_table_names.insert(std::make_pair(toks.at(i), t));
          break;
        }

        case PRINTSTRUC: {
          headTable = tableSearch(toks.at(i));
          printStructure(headTable,5);
          stage = TABLE_LOOKUP;
          break;
        }

        case PRINT: {
          if(PRINT_DEBUG) printf("PRINT: %s\n", toks.at(i).c_str());
          headTable = tableSearch(toks.at(i));
          printTable(headTable);
          stage = TABLE_LOOKUP;
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
          if(GET_DEBUG) printf("GET: %s\n", toks.at(i).c_str());
          std::vector<std::string> keys;
          std::string word;
          // for(int j = 0; j < toks.at(i).length(); j++) {
          //   if(command.at(j) == ','){
          //     keys.push_back(word);
          //     word = "";
          //   } else{
          //     word += command.at(j);
          //   }
          // }
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
          stage = TABLE_LOOKUP;
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
          if(PUT_DEBUG) printf("PUT: %s\n", toks.at(i).c_str());
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
          headTable = tableSearch(toks.at(i));
          break;
        }

        /* Create the new Database */
        case NEWDB: {
          table_t bk;
          bk.table_name = toks.at(i);
          if(NEWDB_DEBUG) printf("NEW DB NAME:%s\n", toks.at(i).c_str());
          database_table.insert(std::make_pair(toks.at(i), bk));
          break;
        }
      }
    }
  }
}
