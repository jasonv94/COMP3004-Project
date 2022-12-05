#include "therapy.h"

Therapy::Therapy(QString newSession, int initialIntensity, QString hertz){
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

//set methods
void Therapy::set_intensity(int newLvl){intensity = newLvl;}
void Therapy::set_record(bool save){record = save;}
