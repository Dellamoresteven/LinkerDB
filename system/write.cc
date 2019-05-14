#include "../include/LinkerDB.h"

std::mutex writeMTX;

/* see write.h */
void writeThreadHandler(std::string command){
  std::thread(writeDataInTable, command).detach();
  // writeDataInTable(command);
}

/* see write.h */
void writeDataInTable(std::string str){
  writeMTX.lock();
  printf("CommandWrite: %s\n",str.c_str());
  std::ofstream fd;
  std::string filename = "logs/";
  std::string word;
  for(auto i : str){
    if(i == '-' || i == ' '){
      filename += (word + ".txt");
      break;
    }
    word += i;
  }
  fd.open(filename.c_str(), std::ofstream::out | std::ofstream::app);
  fd << str;
  fd << ",\n";
  fd.close();
  writeMTX.unlock();
}
