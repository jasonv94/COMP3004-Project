#ifndef HISTORY_H
#define HISTORY_H

#include <QDateTime>

#include "therapy.h"

/*
 * Stores a therapy session along with the time that it started andis added into the database like this.
 */

class history{
private:
    //QString user?
    QDateTime sessionStart;//signifies when the session started
    therapy session;//stores all of the info about the therapy session


public:
    history(QDateTime start,const therapy& treatment);

    QDateTime get_sessionStart() const;
    therapy get_session() const;


};

#endif // HISTORY_H
