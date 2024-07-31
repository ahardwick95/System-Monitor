#include <string>
#include <iomanip> // helps with formating the string to HH:MM:SS
#include <sstream> // helps output the new format as one string
#include "format.h"

using std::string;
using std::ostringstream; // will help to give us one string to return
using std::setw;//setw will set width of each individual string to  2
using std::setfill; //setfill will pad empty sspaces with 0


string Format::ElapsedTime(long seconds) { 
  	long Hours=seconds/3600;// lets get the hours
  	long remainder=seconds%3600; // this will help us get the minutes and seconds
  	long minutes=remainder/60;
  	long actual_seconds=remainder%60;
  
  //lets get the right format for display
  ostringstream myTime;
  myTime<<setw(2)<< setfill('0')<<Hours<<":"
  		<<setw(2)<< setfill('0')<<minutes<<":"
    	<<setw(2)<< setfill('0')<<actual_seconds;
  
 return myTime.str();

}