#include "../include/LinkerDB.h"


/* see read.h */
void read(){
  /* Init up my infile */
  std::ifstream inFile;
  /* Gets Whitespaces and newlines in my input */
  inFile >> std::noskipws;
  char x;
  std::string command = "";
  /* opens the logs directory */
  if (auto dir = opendir("./logs")) {
    /* reads all files in the logs directory */
    while (auto f = readdir(dir)) {
      /* dont wanna read anything that starts with a '.' */
      if (f->d_name[0] == '.'){ continue; }
      /* gettings my file ready */
      std::string res = "./logs/";
      res += f->d_name;

      /* opening from my file */
      inFile.open(res);
      /* getting my filename without the .txt at the end */
      std::string filename = f->d_name;
      filename = filename.substr(0,filename.length()-4);

      /* setting my command to create the new db */
      command = ("newdb " + filename);
      printf("Command = %s\n", command.c_str());
      /* Sending the command to my database handler */
      databaseHandler(command, true);
      /* Reset */
      command = "";
      /* reading from each file (reading into char x) */
      while (inFile >> x) {
        /* ',' tells us the command is over */
        if(x == ','){
          printf("Command = %s\n", command.c_str());
          /* dispatches the command to the hnalder */
          databaseHandler(command, true);
          /* resets command */
          command = " ";
        }else if(x != '\n'){ //Want everything besides ',' and '\n'
          /* makes the commands */
          command += x;
        }
      }
      /* closes that file and gets ready for the next */
      inFile.close();
    }
    /* closes the directory */
    closedir(dir);
  }
}
