
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
          /* Sets up new table */
          table_t * t = new table_t;
          /* adds the table name */
          t->table_name = toks.at(i);
          /* inserts it in the tableHead */
          headTable->linked_table_names.insert(std::make_pair(toks.at(i), t));
          /* Done! */
          break;
        }

        /**
         * Prints a visual interpretation of a database
         *
         * THIS FUNCTION IS NOT YET WORKING SORZ
         */
        case PRINTSTRUC: {
          /* Gets the link you are trying to print the structure of */
          headTable = tableSearch(toks.at(i));
          /* Sets the headpath to the new value */
          headTablePath = toks.at(i);
          /* Checks to make sure it exists */
          if(headTable->table_name == "-1"){ toks.clear(); break; }
          /* Sends in to the printStructure function in print.cc */
          printStructure(headTable,5);
          /* Modifies the stage once print is done */
          stage = TABLE_LOOKUP;
          /* Done! */
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
          /* Gets the link to which you want to print */
          headTable = tableSearch(toks.at(i));
          /* Sets the headpath to the new value */
          headTablePath = toks.at(i);
          /* Checks to make sure it exists */
          if(headTable->table_name == "-1"){ toks.clear(); break; }
          /* Sends in to the printTable function in print.cc */
          printTable(headTable);
          /* Modifies the stage once print is done */
          stage = TABLE_LOOKUP;
          /* Done! */
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
          /* Gets the link to which you want to print */
          headTable = tableSearch(toks.at(i));
          /* Sets the headpath to the new value */
          headTablePath = toks.at(i);
          /* Checks to make sure it exists */
          if(headTable->table_name == "-1"){ toks.clear(); break; }
          /* Sends in to the describeTable function in print.cc */
          describeTable(headTable);
          /* Done! */
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
          /* Tries to find the value you are searching for in the headTable */
          auto checksearch = headTable->data.find(toks.at(i));

          if(checksearch != headTable->data.end()){ /* found */
            if(GET_DEBUG) printf("Found: %s = %s\n", toks.at(i).c_str(), (checksearch->second.str_data).c_str());
            /* Forms response if found */
            resp = ("Found '" + toks.at(i) + "' = '" + checksearch->second.str_data + "' in '" + headTablePath + "'");
          }else{ /* Did not find */
            /* Forms response if not found */
            resp = "Did not find '" + toks.at(i) + "' in '" + headTablePath + "'";
            if(GET_DEBUG) printf("Did not find %s\n", toks.at(i).c_str());
          }
          /* Sends response to formResponse in response.cc */
          formResponse(resp, stage);
          /* Done! */
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
          /* Keys for the incoming Values */
          std::vector<std::string> keys;
          /* values that map to those keys */
          std::vector<std::string> values;
          /* used to know if i should be inserting into keys or values */
          bool isEqualsChar = true;
          /* Starting a i because I dont want to read old tokens */
          /* In general I wanted to keep away from reading past/future tokens
             but I couldnt think of easier ways to get this done */
          for(int j = i; j < toks.size(); j++) {
            /* Making sure the stage has not change, if it has we are in trouble */
            int temp = keyWordSearch(toks.at(j), stage);
            if(stage != temp){ stage = temp; break; }

            if(toks.at(i) == "="){
              isEqualsChar = false;
            }else if(isEqualsChar){
              keys.push_back(toks.at(i));
            }else{
              values.push_back(toks.at(i));
            }
            /* Since i am reading future tokens I want to make sure I do
               not read them later so i increment 'i' */
            i++;
          }
          /* Start my response */
          resp = "Inserted";
          for(int j = 0; j < keys.size(); j++){
            resp += (" '" + keys.at(j) + "' = '" + values.at(j) + "'");
            auto search = headTable->data.find(keys.at(j));
            if(search != headTable->data.end()){ /* already created */
              /* If the value you pair already exsists I just update it so we
                 we dont experience any collisions of data in a link */
              search->second.str_data = values.at(j);
            }else{ /* not created */
              data_t newEntry;
              newEntry.str_data = values.at(j);
              /* If the value pair has not been created already I need to create it
                 and add it to the link */
              headTable->data.insert(std::make_pair(keys.at(j), newEntry));
            }
          }
          resp += " into '" + headTablePath + "'";
          /* Sending my response to my reponse handler */
          formResponse(resp, stage);
          /* Done! */
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
          /* Gets the link to which you want to print */
          headTable = tableSearch(toks.at(i));
          /* Sets the headpath to the new value */
          headTablePath = toks.at(i);
          /* Checks to make sure it exists */
          if(headTable->table_name == "-1"){ resp = "No such link as '" + toks.at(i) + "'";  formResponse(resp, stage); toks.clear(); }
          /* Done! */
          break;
        }

        /* Create the new Database */
        case NEWDB: {
          /* creats a new table_t */
          table_t bk;
          /* Sets the name to the Database name */
          bk.table_name = toks.at(i);
          if(NEWDB_DEBUG) printf("NEW DB NAME:%s\n", toks.at(i).c_str());
          /* insertings the name and the table as a pair into the database_table map */
          database_table.insert(std::make_pair(toks.at(i), bk));
          /* Forms response */
          resp = "Database '" + toks.at(i) + "' was created!";
          /* Sending my response to my reponse handler */
          formResponse(resp, stage);
          /* Done! */
          break;
        }
      }
    }
  }
}
