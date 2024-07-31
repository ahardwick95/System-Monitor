#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() ;                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  Process(int id);
  // TODO: Declare any necessary private members
  //lets make a lot memebr variables so stuff can update properly
 private:
  int myPid; // makes sense as all the functions for this object is based on PID number
  string The_Ram;
  string The_User;
  long int U_Up;//uptime for process
  string The_Cmd;
  float util;
  
};

#endif