#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "../include/process.h"
#include "../include/processor.h"
#include "../include/system.h"
#include "../include/linux_parser.h"

using std::string;
using std::vector;


/**
 * Getter function to return the system's CPU
 * @param none
 * @return system's CPU as a Processor object
*/
Processor& System::Cpu() { return cpu_; }



/**
 * Getter function to return a container composed of the system's processes
 * @param none
 * @return system's processes a vector of Process objects
*/
vector<Process>& System::Processes() { 
  processes_ = {};
  vector<int>pids = LinuxParser::Pids();
  
  for(auto pid : pids){
    Process p(pid);
    processes_.emplace_back(p);
  }
  
  std::sort(processes_.begin(), processes_.end(), [](const Process& a, const Process& b){ return a < b;});
  
  return processes_; 
}

/**
 * This function returns number of CPUs in the system
 * @param none
 * @return int
*/
int System::NumberOfCPUs(){ return LinuxParser::NumberOfCPUs();}


/**
 * This function returns system's kernel identifier
 * @param none
 * @return system's kernel identifier as string
*/
string System::Kernel() { return LinuxParser::Kernel(); }


/**
 * This function returns the amount of used memory on the system
 * @param none
 * @return the system's used memory by calling MemoryUtilization function from LinuxParser namespace 
*/
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }


/**
 * This function returns the operating system name
 * @param none
 * @return name of OS as string
*/
string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }


/**
 * This function returns the number of processes actively running on the system
 * @param none
 * @return the number of processes as an int
*/
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }


/**
 * This function returns the total number of processes on the system
 * @param none
 * @return total number of processes on the system as int type
*/
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }


/**
 * This function returns the number of seconds since the system started running
 * @param none
 * @return the number of seconds as long
*/
long int System::UpTime() { return LinuxParser::UpTime(); }