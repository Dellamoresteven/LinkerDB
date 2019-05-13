/************************************************************
* The tableSearch(std::string) function is a general        *
* function that can accept any table lookup command.        *
* i.e. Rhythm->user->..ect.                                 *
* This function will first look for the database and then   *
* go through the links (->) until it gets to the end or     *
* a link does not exist. If a link OR database does not     *
* exist then it will return a table with the table_name     *
* equal to -1. If it does exisit it will send back a        *
* pointer to the table.                                     *
************************************************************/
table_t * tableSearch(std::string);
