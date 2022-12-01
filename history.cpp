#include "history.h"

History::History(QString date,QString therapyName, QString sessionTime, QString frequency){
    this->date = date;
    this->therapyName = therapyName;
    this->sessionTime = sessionTime;
    this->frequency = frequency;

}

QString History::get_date() {return date;}
QString History::get_therapyName(){return therapyName;}
QString History::get_sessionTime(){return sessionTime;}
QString History::get_frequency(){return frequency;}
//get methods
//QDateTime History::get_sessionStart() const{return sessionStart;}
//Therapy History::get_session() const{return session;}

