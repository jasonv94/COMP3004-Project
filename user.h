#ifndef USER_H
#define USER_H
#include <vector>
#include "history.h"

/*
 *User Class stores the user name and the history for that user
 *
 *
 */

class User{
private:
    QString name;//stores the name of the user
    std::vector <history> records;//stores all of the individual treatments that have been saved
public:
    User(QString username);

    QString get_name() const;
    std::vector<history> get_history();
};

#endif // USER_H
