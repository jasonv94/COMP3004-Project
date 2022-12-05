#include "history.h"


History::History(QString date,QString therapyName, int session_id, QString sessionTime, QString frequency,int intensity){
    this->date = date;
    this->therapyName = therapyName;
    this->session_id = session_id;
    this->sessionTime = sessionTime;
    this->frequency = frequency;
    this->intensity = intensity;

}

QString History::get_date() {return date;}
QString History::get_therapyName(){return therapyName;}
QString History::get_sessionTime(){return sessionTime;}
QString History::get_frequency(){return frequency;}
QString History::string_record(){return QString::number(session_id);}
int History::get_sessionID(){return session_id;}
QString History::get_intensity(){return QString::number(intensity);}
