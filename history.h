#ifndef HISTORY_H
#define HISTORY_H

#include <QDateTime>

#include "therapy.h"

/*
 * Stores a therapy session along with the time that it started andis added into the database like this.
 */

class History{
private:
    //QString user?
    QDateTime sessionStart;//signifies when the session started
    //Therapy session;//stores all of the info about the therapy session


public:
    History(QDateTime start);

    QDateTime get_sessionStart() const;
    Therapy get_session() const;


};

#endif // HISTORY_H
