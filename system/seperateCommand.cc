#include "../include/LinkerDB.h"

void seperateCommand(std::string input, std::vector<std::string> & v){
  std::string temp = "";
  for(int i = 0; i < input.length(); i++){
    if(input.at(i) == ' ' || input.at(i) == ','){
      if(temp != ""){ v.push_back(temp); }

      temp = "";
    }else if(input.at(i) == '='){
      if(temp != ""){ v.push_back(temp); }
      v.push_back("=");
      temp = "";
    } else {
      if(input.at(i) != ' '){ temp += input.at(i); }
    }
  }
  if(temp.length() != 0){ v.push_back(temp); }
  return;
}
