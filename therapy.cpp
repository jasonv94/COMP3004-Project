#include "therapy.h"

Therapy::Therapy(QString newSession, int initialIntensity, QString hertz, int t){
   length = t;
   duration = new QTimer();
   session = newSession;
   intensity = initialIntensity;
   record = false;
   frequency = hertz;

}
//get methods
QTimer* Therapy::get_duration() const{return duration;}
QString Therapy::get_name() const{return session;}
QString Therapy::get_frequency() const{return frequency;}
int Therapy::get_intensity() const{return intensity;}
bool Therapy::get_record() const{return record;}
int Therapy::get_length() const{return length;}

//set methods
void Therapy::set_intensity(int newLvl){intensity = newLvl;}
void Therapy::set_record(bool save){record = save;}
void Therapy::set_time(int t){length = t;}
