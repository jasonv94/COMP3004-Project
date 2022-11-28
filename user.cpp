#include "user.h"

User::User(QString username){
    name = username;
    std::vector <History> records;
}

//get methods
QString User::get_name() const {return name;}
std::vector<History> User::get_history(){return records;};
