#ifndef THERAPY_H
#define THERAPY_H

#include <QString>
#include <QTimer>

/*
 * This class stores all of hte basic information for each treatment session
 */

class Therapy{
private:
    int length;
    QTimer* duration;//duration of the session
    QString session;//the name of the session that is being used
    int intensity;//stores an int between 1 and 8 inclusive that tells how intense the therapy session is
    bool record;//if set to true then the therapy session is recorded
    QString frequency;//stores the frequency that the program runs at
public:
    Therapy(QString newSession, int initialIntensity, QString hertz, int t);

    QTimer* get_duration() const;
    QString get_name() const;
    QString get_frequency() const;
    int get_intensity() const;
    bool get_record() const;
    int get_length() const;

    void set_intensity(int newLvl);
    void set_record(bool save);
    void set_time(int t);
};

#endif // THERAPY_H
