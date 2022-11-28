#include "history.h"

history::history(QDateTime start, const therapy& treatment){
    sessionStart = start;
    session = treatment;
}

//get methods
QDateTime history::get_sessionStart() const{return sessionStart;}
therapy history::get_session() const{return session;}
