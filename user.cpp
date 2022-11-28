#include "user.h"

User::User(QString username){
    name = username;
    std::vector <history> records;
}

//get methods
QString User::get_name() const {return name;}
std::vector<history> User::get_history(){return records;};
