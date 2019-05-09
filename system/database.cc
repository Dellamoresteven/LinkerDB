
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
  } else if(key == "PRINTSTRUC"){
    return PRINTSTRUC;
  } else if(key == "EXIT"){
    write();
  } else if(key == "DESC"){
    return DESC;
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
  /* This is the response being sent over to response.cc */
  std::string resp;
  for(;;){
    if(COMMAND_START_DEBUG) printf("Command: \n");
    resp = "";
    /* Grabs user command in String format */
    getline(std::cin, input);
    /* Clear my token array out for the next command */
    toks.clear();
    /* clear my resp token */
    resp = "";
    /* This function will seperate the strings into the tokens for easy use */
    seperateCommand(input, toks);

    if(COMMAND_START_DEBUG){
      for(auto i : toks){
        printf("Toks: %s\n", i.c_str());
      }
    }
    /* This is the head table that all the functions will look at */
    table_t * headTable;
    /* This is that path, used for response/debug reasons */
    std::string headTablePath;

    /* init new command to COMMAND_START */
    int stage = TABLE_LOOKUP;

    /* Runs through all the tokens */
    for(int i = 0; i < toks.size(); i++){
      /* Prints the DB's (For testing Only) */
      if(toks.at(i) == "printDB"){
        printDB();
        break;
      }

      /* Changes the stage if need be */
      int newStage = keyWordSearch(toks.at(i), stage);

      /* If the stage is subject to change */
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

        /**
         * Prints a visual interpretation of a database
         *
         * THIS FUNCTION IS NOT YET WORKING SORZ
         */
        case PRINTSTRUC: {
          headTable = tableSearch(toks.at(i));
          headTablePath = toks.at(i);
          if(headTable->table_name == "-1"){ toks.clear(); break; }
          printStructure(headTable,5);
          stage = TABLE_LOOKUP;
          break;
        }

        /**
         * Prints all contents in the link specfied
         *
         * example: print Rhythm
         * Prints all data inside Rhythm
         * example: print Rhythm->user
         * prints all contents inside Rhythm DB and user link
         */
        case PRINT: {
          if(PRINT_DEBUG) printf("PRINT: %s\n", toks.at(i).c_str());
          headTable = tableSearch(toks.at(i));
          headTablePath = toks.at(i);
          if(headTable->table_name == "-1"){ toks.clear(); break; }
          printTable(headTable);
          stage = TABLE_LOOKUP;
          break;
        }

        /**
         * Prints all the links inside a given table
         *
         * example: print Rhythm
         * Prints all links inside rhythm (like user)
         * example: print Rhythm->user
         * prints all links inside of Rhythm, user link
         */
        case DESC: {
          headTable = tableSearch(toks.at(i));
          headTablePath = toks.at(i);
          if(headTable->table_name == "-1"){ toks.clear(); break; }
          describeTable(headTable);
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
            if(GET_DEBUG) printf("Found: %s = %s\n", toks.at(i).c_str(), (checksearch->second.str_data).c_str());
            resp = ("Found '" + toks.at(i) + "' = '" + checksearch->second.str_data + "' in '" + headTablePath + "'");
          }else{ /* Did not find */
            resp = "Did not find '" + toks.at(i) + "' in '" + headTablePath + "'";
            if(GET_DEBUG) printf("Did not find %s\n", toks.at(i).c_str());
          }
          formResponse(resp, stage);
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
          resp = "Inserted";
          for(int j = 0; j < keys.size(); j++){
            resp += (" '" + keys.at(j) + "' = '" + values.at(j) + "'");
            auto search = headTable->data.find(keys.at(j));
            if(search != headTable->data.end()){ /* already created */
              search->second.str_data = values.at(j);
            }else{ /* not created */
              data_t newEntry;
              newEntry.str_data = values.at(j);
              headTable->data.insert(std::make_pair(keys.at(j), newEntry));
            }
          }
          resp += " into '" + headTablePath + "'";
          formResponse(resp, stage);
          // printf("%s", resp("Inserted\n").c_str());
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
          headTablePath = toks.at(i);
          if(headTable->table_name == "-1"){ resp = "No such link as '" + toks.at(i) + "'";  formResponse(resp, stage); toks.clear(); }
          break;
        }

        /* Create the new Database */
        case NEWDB: {
          table_t bk;
          bk.table_name = toks.at(i);
          if(NEWDB_DEBUG) printf("NEW DB NAME:%s\n", toks.at(i).c_str());
          database_table.insert(std::make_pair(toks.at(i), bk));
          resp = "Database '" + toks.at(i) + "' was created!";
          formResponse(resp, stage);
          break;
        }
      }
    }
  }
}
