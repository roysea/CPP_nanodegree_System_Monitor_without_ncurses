#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "../include/process.h"
#include "../include/linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) 
  	:pid_(pid), cpu_usage_(CpuUtilization()){}

/*
    Getter function returns this process's ID
    @param None
    @return Process private member variable pid_
*/ 
int Process::Pid() { return pid_; }


/*
    Return this process's CPU utilization
    @param None
    @return cpu usage as float
*/ 
float Process::CpuUtilization() { 
  
 long active_jiffies = LinuxParser::ActiveJiffies(pid_);
 long seconds = LinuxParser::UpTime() - UpTime();
  
 return ((float)active_jiffies / sysconf(_SC_CLK_TCK) / seconds); }


/*
    Return the the command that generated this process
    @param None
    @return string
*/ 
string Process::Command() { return LinuxParser::Command(Pid()); }


/*
    Return this process's memory utilization
    @param None
    @return string
*/ 
string Process::Ram() { return LinuxParser::Ram(Pid()); }


/*
    Return the user (name) that generated this process
    @param None
    @return username of the current process_
*/ 
string Process::User() { return LinuxParser::User(Pid());}


/*
    Return the age of this process (in seconds)
    @param None
    @return long
*/ 
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }


/*
    This function Overload the "less than" comparison operator for Process objects
    @param None
    @return return true if the other Process has less cpu usage then the current process
*/ 
bool Process::operator<(Process const& a) const { 

	return this->cpu_usage_ > a.cpu_usage_;
}


