#include "therapy.h"

Therapy::Therapy(int duration, QString newSession, int initialIntensity, bool save){
   length = duration;
   session = newSession;
   intensity = initialIntensity;
   record = save;

}
//get methods
int Therapy::get_length() const{return length;}
QString Therapy::get_name() const{return session;}
int Therapy::get_intensity() const{return intensity;}
bool Therapy::get_record() const{return record;}

//set methods
void Therapy::set_intensity(int newLvl){intensity = newLvl;}
void Therapy::set_record(bool save){record = save;}
