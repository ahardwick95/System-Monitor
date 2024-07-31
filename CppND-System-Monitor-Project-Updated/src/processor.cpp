#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization

/* PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald*/

//based on a stack overflow post
 

float Processor::Utilization() {
  //previous values
 //std::vector<std::string> cpu_values=LinuxParser::CpuUtilization();
//IdelJiffies() is iowait and idle
//NonIdle is ActiveJiffies()
 Idle_delta=LinuxParser::IdleJiffies()-prevIdle;
 Tot_delta=LinuxParser::Jiffies()-prevTotal;
  
 prevIdle=LinuxParser::IdleJiffies();
 prevTotal=LinuxParser::Jiffies();
  
return (Tot_delta-Idle_delta)/Tot_delta;



}