#ifndef HISTORY_H
#define HISTORY_H

#include <QDateTime>

#include "therapy.h"

/*
 * Stores a therapy session along with the time that it started andis added into the database like this.
 */

class History{
private:
    QString date;//change this
    int session_id;
    QString therapyName;
    QString sessionTime;
    QString frequency;
    int intensity;
    //QString user?
    //QDateTime sessionStart;//signifies when the session started
    //Therapy session;//stores all of the info about the therapy session


public:
    History(QString date,QString therapyName, int session_id,QString sessionTime, QString frequency,int intensity);// change to datetime later
    QString get_date();
    QString get_therapyName();
    QString get_sessionTime();
    QString get_frequency();
    QString string_record();
    int get_sessionID();
    QDateTime get_sessionStart() const;
    Therapy get_session() const;
    QString get_intensity();


};

#endif // HISTORY_H
