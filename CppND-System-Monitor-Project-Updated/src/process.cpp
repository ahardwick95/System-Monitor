#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

Process::Process(int id){
  myPid=id;
    
}//this is process's ID
int Process::Pid() { return myPid; }// a getter function for the private variable we made

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  
 float pid_Act=static_cast<float>(LinuxParser::ActiveJiffies(Pid()));
 float pid_Uptime=static_cast<float>(LinuxParser::UpTime(Pid()));  //process uptime= system uptime - process startime
 return (pid_Act/pid_Uptime)*100;

} // based on research , active jiffies /uptime of process is the cpu usage , in other words, how active has the process been since it started = cpu usgae

// TODO: Return the command that generated this process
string Process::Command() { 
  The_Cmd=LinuxParser::Command(Pid()); 
  return The_Cmd;         
                       }
// TODO: Return this process's memory utilization
string Process::Ram() {
  The_Ram=LinuxParser::Ram(Pid());
  return The_Ram;
          }
// TODO: Return the user (name) that generated this process
string Process::User() { 
  The_User=LinuxParser::User(Pid());
  return The_User;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  U_Up=LinuxParser::UpTime(Pid()); 
  return U_Up;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
//apprantly processes can be sorted on several factors such as cpu usage percentage or ram , or pid number
bool Process::operator<(Process const& a) const {
  return The_Ram<a.The_Ram; //apprantly i dont have to cast to long to comapre values, learned something new
}