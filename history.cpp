#include "history.h"

History::History(QDateTime start){
    sessionStart = start;

}

//get methods
QDateTime History::get_sessionStart() const{return sessionStart;}
//Therapy History::get_session() const{return session;}
