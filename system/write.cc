#include "../include/LinkerDB.h"


void write(){
  for ( auto it = database_table.begin(); it != database_table.end(); ++it ){
    std::ofstream fd;
    std::string filename = "logs/" + it->first + ".txt";
    fd.open(filename.c_str());

    table_t * table = (it->second);

    writeDataInTable(table, fd);

    fd.close();
  }
  exit(1);
}

void writeDataInTable(table_t * table, std::ofstream & fd){
  fd << table->table_name << "\n";
  for ( auto it = table->data.begin(); it != table->data.end(); ++it ){
    fd << it->first << "=" << it->second.str_data << "\n";
  }
  for ( auto it = table->linked_table_names.begin(); it != table->linked_table_names.end(); ++it ){
    writeDataInTable(it->second, fd);
  }
  return;
}
