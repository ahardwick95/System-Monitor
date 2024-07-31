#include <dirent.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
using std::string;
using std::cout;
using std::vector;
using std::stof;
using std::to_string;

#include "linux_parser.h"
#include "format.h"


// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());// open /proc direcory
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  //memory utilization is % is (1- MemAvailable/Memtotal)*100
  float MemAvailable,TotalMem,systemMemoryUtil;
  string name,number,memSize;
  vector<string> memoryUsage;
  std::stringstream ss;
  std::stringstream xx;
  std::stringstream zz;
  string line;
  int count=0;
  std::ifstream stream("/proc/meminfo"); //open the /proc/meminfo file
    
 	// go through first 3 lines of file, and gather the right numbers
    while(count<3 &&  std::getline(stream, line)){
    std::istringstream linestream(line);
    linestream >> name >> number >> memSize;
    memoryUsage.push_back(number);
    count++;
    }
    stream.close();
  //couldnt use stof for some reason , so i used stringstream to convert string to float and set it to 2 sig figs
  ss<<memoryUsage[0]; //memtotal
  ss>>TotalMem;
  xx<<memoryUsage[2];//memfree
  xx>>MemAvailable;
  systemMemoryUtil=((TotalMem-MemAvailable)/TotalMem); //based on stack overlow
  //zz<<std::fixed<<std::setprecision(1)<<temp;
  return systemMemoryUtil;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {   //does not inlcude 2 sigs after decimal point
  
    long myUptime;
    string temp_1,temp_2;
    string line;
    std::ifstream stream("/proc/uptime"); //open the /proc/meminfo file
    
    while(std::getline(stream, line)){
    	std::istringstream linestream(line);
    	linestream >> temp_1>> temp_2;
    }
    //std::cout<<temp_1<<" "<<temp_2<<"\n";
    stream.close();
    	myUptime=stol(temp_1);
    //idle_time=stol(temp_2);
    	return myUptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  
    long Tot_Jiffs;
    string temp_1,user_Jiffs,nice_jiffs,sys_Jiffs,idle_Jiffs,iowait_Jiffs,irq_Jiffs,softirq_Jiffs;
    string line;
    std::ifstream stream("/proc/stat"); //open the /proc/stat file
  
    int count=0;
  //the first line of the file contains all the info need for total jifs, active jiffs and idle jiffs
    while(std::getline(stream, line) and count<1){
    	std::istringstream linestream(line);
    	linestream >> temp_1>> user_Jiffs>>nice_jiffs>>sys_Jiffs>>idle_Jiffs>>iowait_Jiffs>>irq_Jiffs>>softirq_Jiffs;  // this euqals the total system time
        count++;
    }
    stream.close();
  try{
 	 Tot_Jiffs=stol(user_Jiffs) +stol(nice_jiffs) +stol(sys_Jiffs) +stol(idle_Jiffs) +stol(iowait_Jiffs) +stol(irq_Jiffs) +stol(softirq_Jiffs) ; // this is the number of ticks , we need seconds
  }catch(std::invalid_argument& e){
    Tot_Jiffs=0;
  }
  	 Tot_Jiffs=Tot_Jiffs/sysconf(_SC_CLK_TCK);
     return Tot_Jiffs;


}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
    long Total_time; // will hold the total active jiffies for a process
    vector<string> proc_stats;
    string data;
    char delimiter{' '};
    std::ifstream stream("/proc/"+to_string(pid)+"/stat"); //open the /proc/stat file
 
   while( std::getline(stream,data,delimiter)){
  	proc_stats.push_back(data);
  
   }
  // im assouming for each process we also want the time for each child process as well
  //therefore i will include cutime and cstime
  //based on documentation ,14th val is utime, 15th is stime, 16th is cutime and 17th is cstime
    stream.close();
  try{
  Total_time=(stol(proc_stats[13]) + stol(proc_stats[14])+stol(proc_stats[15])+stol(proc_stats[16]))/sysconf(_SC_CLK_TCK);
  }catch(std::invalid_argument& e){
    Total_time=0;
  }
   return Total_time;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
long Act_Jiffs;
     string temp_1,temp_2,user_Jiffs,nice_jiffs,sys_Jiffs,iowait_Jiffs,irq_Jiffs,softirq_Jiffs,steal_Jiffs,Guest_Jiffs,GuestNice_Jiffs;
    string line;
    std::ifstream stream("/proc/stat"); //open the /proc/stat file
  long temp;
    int count=0;
  //the first line of the file contains all the info need for total jifs, active jiffs and idle jiffs
    while(std::getline(stream, line) and count<1){
    	std::istringstream linestream(line);
    	linestream >> temp_1>> user_Jiffs>>nice_jiffs>>sys_Jiffs>>temp_2>>iowait_Jiffs>>irq_Jiffs>>softirq_Jiffs>>steal_Jiffs>>Guest_Jiffs>>GuestNice_Jiffs;  // this euqals the total system time
        count++;
    }
    stream.close();
  try{
 	 temp=stol(user_Jiffs) +stol(nice_jiffs) +stol(sys_Jiffs) +stol(iowait_Jiffs) +stol(irq_Jiffs) +stol(softirq_Jiffs) ; // total jiffs - idle jifs = Active jiffs for the system
  }catch(std::invalid_argument& e){
     temp=0;
  }
  	 Act_Jiffs=temp/sysconf(_SC_CLK_TCK);
     return Act_Jiffs;

}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 

long idle_Jiffs;
    string temp_1,temp_2,user_Jiffs,nice_jiffs,sys_Jiffs,iowait_Jiffs,irq_Jiffs,softirq_Jiffs,steal_Jiffs,Guest_Jiffs,GuestNice_Jiffs;
    string line;
    std::ifstream stream("/proc/stat"); //open the /proc/stat file
  
    int count=0;
  //the first line of the file contains all the info need for total jifs, active jiffs and idle jiffs
    while(std::getline(stream, line) and count<1){
    	std::istringstream linestream(line);
    	linestream >> temp_1>> user_Jiffs>>nice_jiffs>>sys_Jiffs>>temp_2>>iowait_Jiffs>>irq_Jiffs>>softirq_Jiffs>>steal_Jiffs>>Guest_Jiffs>>GuestNice_Jiffs;  // temp_2 holds idle time, total idle time is idle time + iowait
        count++;
    }
    stream.close();
  try{
  	 idle_Jiffs=(stol(temp_2)+stol(iowait_Jiffs))/sysconf(_SC_CLK_TCK);
  }catch(std::invalid_argument& e){
    idle_Jiffs=0;
  }
     return idle_Jiffs;

}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
//From what i gathered, cpu utilization is active jiffies + IdleJiffies
  // we need to calculate prev and curr values in orderto get percentage
    vector <string> cpu_Values;
	string data;
  	std::ifstream stream("/proc/stat");
  	std::getline(stream,data);
  
      if(data.find("cpu")==0){
       	std::istringstream linestream(data);
        while(linestream >> data){
  			cpu_Values.push_back(data);
        }
      }
          stream.close();
	return cpu_Values;


}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {

int Tot_Procs=0;
    string temp_1;
    string temp_2;//will help get the numbber of processes
    string line;
    std::ifstream stream("/proc/stat"); //open the /proc/stat file

    while(std::getline(stream, line)){
    	
    	if(line.find("processes")==0){// stop on the line that has processes
          std::istringstream linestream(line);
          linestream>>temp_1>>temp_2;
        }
    }
    stream.close();
  try{
  	 Tot_Procs=stoi(temp_2);
  }catch(std::invalid_argument& e){
    Tot_Procs=0;
  }
     return Tot_Procs;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
 int Run_Procs=0;
    string temp_1;
    string temp_2;//will help get the number of running processes
    string line;
    std::ifstream stream("/proc/stat"); //open the /proc/stat file
    while(std::getline(stream, line)){
    	
    	if(line.find("procs_running")==0){// stop on the line that has procs_running
          std::istringstream linestream(line);
          linestream>>temp_1>>temp_2;
        }
    }
    stream.close();
  try{
  	 Run_Procs=stoi(temp_2);//convert string to int
  }catch(std::invalid_argument& e){
    Run_Procs=0;
  }
     return Run_Procs;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 

	string CMD;
    string line;
	std::ifstream stream("/proc/"+to_string(pid)+"/cmdline"); //open the /proc/pid/cmdline file
    std::getline(stream, CMD);
    stream.close();
    return CMD;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    string ram_Mb;
    string line;
    string temp_1;
  std::string ram_Kb;
	std::ifstream stream("/proc/"+to_string(pid)+"/status"); //open the /proc/stat file
    while(std::getline(stream, line)){
    	if(line.find("VmSize")==0){// stop on the line that has procs_running
          std::istringstream linestream(line);
          linestream>>temp_1>>ram_Kb;
        }
    }
    stream.close();
  try{
  	ram_Mb=to_string( int(std::stof(ram_Kb)/float(1024))) ;
  }catch(std::invalid_argument& e){
    ram_Mb="0";
  }
  	return ram_Mb;
 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { //	TEST THIS OUT 
	string UID;
    string line;
    string temp_1;
	std::ifstream stream("/proc/"+to_string(pid)+"/status"); //open the /proc/stat file
    while(std::getline(stream, line)){
    	
    	if(line.find("Uid")==0){// stop on the line that has procs_running
          std::istringstream linestream(line);
          linestream>>temp_1>>UID;
        }
    }
  	  stream.close();
    return UID;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
    string temp=LinuxParser::Uid(pid);
    string line;
   	string temp_1,temp_2,z;
	std::ifstream stream("/etc/passwd"); //open the /proc/stat file
   
    while(std::getline(stream, line)){
    	std::stringstream data_grabber(line);
        std::string username, passwd,UID;
      
      	std::getline(data_grabber,username, ':');
      	std::getline(data_grabber,passwd, ':');
      	std::getline(data_grabber,UID, ':');
      	if(UID==temp){
        	return username;
      	}    
     }
    stream.close();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  //process uptime is system uptime - proccess start time
    vector<string> proc_stats;
    string data;
    char delimiter{' '};
    std::ifstream stream("/proc/"+to_string(pid)+"/stat"); //open the /proc/stat file
 
   while( std::getline(stream,data,delimiter)){
  	proc_stats.push_back(data);
  
   }
  stream.close();
  long proc_start=stol(proc_stats[21])/sysconf(_SC_CLK_TCK); // start time is 22nd value in stats for a process
  long Proc_Uptime=LinuxParser::UpTime()-proc_start;
  return Proc_Uptime;

}
