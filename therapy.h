#ifndef THERAPY_H
#define THERAPY_H

#include <QString>

/*
 * This class stores all of hte basic information for each treatment session
 */

class Therapy{
private:
    int length;//duration of the session
    QString session;//the name of the session that is being used
    int intensity;//stores an int between 1 and 8 inclusive that tells how intense the therapy session is
    bool record;//if set to true then the therapy session is recorded
public:
    Therapy(int duration, QString newSession, int initialIntensity, bool save);

    int get_length() const;
    QString get_name() const;
    int get_intensity() const;
    bool get_record() const;

    void set_intensity(int newLvl);
    void set_record(bool save);
};

#endif // THERAPY_H
