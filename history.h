#ifndef HISTORY_H
#define HISTORY_H

#include <QDateTime>

#include "therapy.h"

/* Purpose:
 * Stores a therapy session along with the time that was initialized
 * Variables/functions describe their respective purpose
 *
 */

class History{
private:
    QString date;
    int session_id;
    QString therapyName;
    QString sessionTime;
    QString frequency;
    int intensity;

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
    QString toString()const;


};

#endif // HISTORY_H
