#include "history.h"


History::History(QString date,QString therapyName, int session_id, QString sessionTime, QString frequency,int intensity){
    this->date = date;
    this->therapyName = therapyName;
    this->session_id = session_id;
    this->sessionTime = sessionTime;
    this->frequency = frequency;
    this->intensity = intensity;

}

/*
 * Function: get_date
 * returns class variable
 * Returns:
 *  date
 *
 */
QString History::get_date() {return date;}

/*
 * Function: get_therapyName
 * returns class variable
 * Returns:
 *  date
 *
 */
QString History::get_therapyName(){return therapyName;}

/*
 * Function: get_sessionTime
 * returns class variable
 * Returns:
 *  sessionTime
 *
 */
QString History::get_sessionTime(){return sessionTime;}
/*
 * Function: get_frequency
 * returns class variable
 * Returns:
 *  frequency
 *
 */
QString History::get_frequency(){return frequency;}

/*
 * Function: string_record
 * returns class
 * Returns:
 *  string of session_id for GUI
 *
 */
QString History::string_record(){return QString::number(session_id);}

/*
 * Function: get_sessionID
 * returns class
 * Returns:
 *  session_id
 *
 */
int History::get_sessionID(){return session_id;}

/*
 * Function: get_intensity
 * returns class
 * Returns:
 *  string of intensity for GUI
 *
 */
QString History::get_intensity(){return QString::number(intensity);}

