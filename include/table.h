
/* this is where all the data for a given table will be held */
struct data_t {
  // std::vector<data_e> data;
  std::string str_data;
};

/* this is the table */
struct table_t {
  std::string table_name; /* name of the given table */
  std::unordered_map<std::string, table_t> linked_table_names; /* Links to other tables */
  std::unordered_map<std::string, data_t> data; /* Data inside this table */
};
