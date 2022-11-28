#include "therapy.h"

therapy::therapy(int duration, QString newSession, int initialIntensity, bool save){
   length = duration;
   session = newSession;
   intensity = initialIntensity;
   record = save;

}
//get methods
int therapy::get_length() const{return length;}
QString therapy::get_name() const{return session;}
int therapy::get_intensity() const{return intensity;}
bool therapy::get_record() const{return record;}

//set methods
void therapy::set_intensity(int newLvl){intensity = newLvl;}
void therapy::set_record(bool save){record = save;}
