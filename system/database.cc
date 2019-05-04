
#include "../include/LinkerDB.h"



int keyWordSearch(std::string key, int stage){
  /* checks for keywords in the command */

  /* Changes key to a fully uppercase version */
  std::transform(key.begin(), key.end(),key.begin(), ::toupper);
  /* Checks Keywords */
  if(key == "NEWDB"){
    return NEWDB;
  } else if(key == "PUT"){
    return PUT;
  } else if(key == "GET"){
    return GET;
  } else if(key == "PRINT"){
    return PRINT;
  } else if(key == "NEWLINK"){
    return NEWLINK;
  } else if(key == "printStruc"){
    return PRINTSTRUC;
  } else if(key == "EXIT"){
    write();
  }
  /* return the current stage if keyword is not found */
  return stage;
}



int main(void){

  /* Init all system values (including reading from logs) */
  initDB();

  /* Full command */
  std::string input;
  /* Tokens vector */
  std::vector<std::string> toks;
  while(true){
    if(COMMAND_START_DEBUG) printf("Command: \n");
    getline(std::cin, input);
    toks.clear();
    seperateCommand(input, toks);
    if(COMMAND_START_DEBUG){
      for(auto i : toks){
        printf("Toks: %s\n", i.c_str());
      }
    }
    table_t * headTable;

    /* init new command to COMMAND_START */
    int stage = TABLE_LOOKUP;

    /* Runs through all the tokens */
    for(int i = 0; i < toks.size(); i++){
      /* Prints the DB's (For testing Only) */
      if(toks.at(i) == "printDB"){
        printDB();
        break;
      }

      int newStage = keyWordSearch(toks.at(i), stage);

      if(newStage != stage) {stage = newStage; toks.at(i) = ""; continue;}

      /* A switch statment to know where in the command you should be */
      switch(stage){

        /**
         * Creating a new link
         *
         * example: Rhythm NEWLINK user
         * This will make Rhythm->user exist
         *
         * example: Rhythm->user NEWLINK steven
         * This will make Rhythm->user->steven exist
         */
        case NEWLINK: {
          table_t * t = new table_t;
          t->table_name = toks.at(i);
          headTable->linked_table_names.insert(std::make_pair(toks.at(i), t));
          break;
        }

        case PRINTSTRUC: {
          headTable = tableSearch(toks.at(i));
          if(headTable->table_name == "-1"){ toks.clear(); }
          printStructure(headTable,5);
          stage = TABLE_LOOKUP;
          break;
        }

        case PRINT: {
          if(PRINT_DEBUG) printf("PRINT: %s\n", toks.at(i).c_str());
          headTable = tableSearch(toks.at(i));
          if(headTable->table_name == "-1"){ toks.clear(); }
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

          auto checksearch = headTable->data.find(toks.at(i));

          if(checksearch != headTable->data.end()){ /* found */
            printf("Found: %s = %s\n", toks.at(i).c_str(), (checksearch->second.str_data).c_str());
          }else{ /* Did not find */
            printf("Did not find %s\n", toks.at(i).c_str());
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
          if(PUT_DEBUG) printf("PUT: %s\n", toks.at(i).c_str());

          std::vector<std::string> keys;
          std::vector<std::string> values;

          bool isEqualsChar = true;

          for(int j = i; j < toks.size(); j++) {
            int temp = keyWordSearch(toks.at(j), stage);
            if(stage != temp){ stage = temp; break; }
            if(toks.at(i) == "="){
              isEqualsChar = false;
            }else if(isEqualsChar){
              keys.push_back(toks.at(i));
            }else{
              values.push_back(toks.at(i));
            }
            i++;
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
          if(headTable->table_name == "-1"){ toks.clear(); }
          break;
        }

        /* Create the new Database */
        case NEWDB: {
          table_t bk;
          bk.table_name = toks.at(i);
          if(NEWDB_DEBUG) printf("NEW DB NAME:%s\n", toks.at(i).c_str());
          database_table.insert(std::make_pair(toks.at(i), bk));
          formResponse(toks.at(i), stage);
          break;
        }
      }
    }
  }
}
