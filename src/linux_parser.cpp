#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "../include/linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::replace;
using std::istringstream;
using std::ifstream;
using std::getline;
using std::stoi;
using std::stol;

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  
  string line, key, value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    
    while (std::getline(filestream, line)) {
      
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  
  string line, os, version, kernel;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}


// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
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


/**
 * This function returns the amount of used memory on the system
 * @param none
 * @return the system's used memory by calling MemoryUtilization function from LinuxParser namespace 
*/
float LinuxParser::MemoryUtilization() { 
  
  string line, key, value, kb;
  int mem_total, mem_free;
  ifstream filestream(kProcDirectory + kMeminfoFilename);
  if(filestream.is_open()){
    
    while(getline(filestream, line)){
      
      istringstream linestream(line);
      while(linestream >> key >> value >> kb){
        
        if(key == "MemTotal:") mem_total = stoi(value);
        else if(key == "MemFree:") mem_free = stoi(value);
      }
    }
  }
  return (mem_total - mem_free)/(float)(mem_total); 
  }


/**
 * This function returns the number of seconds since the system started running
 * @param none
 * @return the number of seconds as long
*/
long LinuxParser::UpTime() { 

  string line, uptime;
  long seconds = 0;

  ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open()){
    filestream >> uptime;
    seconds = stol(uptime);
  }
  return seconds; }


/**
 * This function returns the number of jiffies for the system
 * @param none
 * @return the number of seconds as long
*/
long LinuxParser::Jiffies(string cpu) {
  return IdleJiffies(cpu) + ActiveJiffies(cpu); }


/*
  This function reads and return the number of active jiffies for the system.
  The global variable jiffies holds the number of ticks that have occurred since the system booted.
  jiffies = (seconds * HZ)
  
  @param None
  @return a long (active jiffies for the system)
*/
long LinuxParser::ActiveJiffies(string cpu) { 
  vector<string> cpu_stats =  CpuUtilization(cpu);
  
    /*  0       1        2       3        4      5     6       7      8       9     
     user    nice   system  idle      iowait  irq   softirq  steal  guest  guest_nice
cpu  74608   2520   24433   1117073   6176    4054    0        0      0      0
*/
  
  //All Active Times = user + nice + system + irq + softirq + steal
  return stol(cpu_stats[0]) + stol(cpu_stats[1]) + stol(cpu_stats[2]) + 
    	 stol(cpu_stats[5]) + stol(cpu_stats[6]) + stol(cpu_stats[7]); }


/*
  This function reads and return the number of idle jiffies for the system
  @param None
  @return a long (idle jiffies for the system)
*/
long LinuxParser::IdleJiffies(string cpu) { 
    vector<string> cpu_stats =  CpuUtilization(cpu);
  
    /*  0       1        2       3        4      5     6       7      8       9     
     user    nice   system  idle      iowait  irq   softirq  steal  guest  guest_nice
cpu  74608   2520   24433   1117073   6176    4054    0        0      0      0
*/
  
  //Idle time  = ioWait + idleTime
  return stol(cpu_stats[3]) + stol(cpu_stats[4]); }


/*
  This function reads and return the number of active jiffies for a PID
  @param None
  @return a long 
*/
long LinuxParser::ActiveJiffies(int pid) { 
  /*  -cat /proc/1590/stat

1590 (bash) S 42 1590 1590 34816 2772 4194304 243 172556 0 0 0(index 13) 0(index 14) 415(index 15) 68(index 16) 20 0 1 0 50369 18460672 687 18446744073709551615 4194304 5192964 140730992542192 0 0 0 65536 4100 65538 1 0 0 17 0 0 0 0 0 0 7290352 7326856 14143488 140730992545081 140730992545347 140730992545347 140730992545774 0
*/
  string value;
  long active_jiffies;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    
    int count = 0;
    while(filestream >> value){ 
      
      if(count == 13) active_jiffies += stol(value);
      else if(count == 14) active_jiffies += stol(value);
      else if(count == 15) active_jiffies += stol(value);
      else if(count == 16) {
        active_jiffies += stol(value);
        break;
      }
      count++;
    }
  }
  
  return active_jiffies; }

/*
  This function get number of CPUs in the machine
  @param None
  @return an int
*/
int LinuxParser::NumberOfCPUs(){
  string line, key, junk, num_cpus;

  ifstream filestream(kProcDirectory + kCpuinfoFilename);
  if(filestream.is_open()){
    
    while(getline(filestream, line)){
      
      istringstream linestream (line);
      linestream >>  key;
      if(key.find("siblings") == 0) linestream >> junk >> num_cpus;
      
      }
   } 
  
 return stoi(num_cpus);
}
/*
  This function reads all CPUs time and return each CPU utilization
  @param None
  @return a string vector: position 0 has the aggregate data and 
  others are as many CPUs that are available in the machine
*/
vector<string> LinuxParser::CpuUtilization(string cpu) { 
  
  string line, key;
  vector<string> CPUs;

  ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    
    while(getline(filestream, line)){
      
      istringstream linestream (line);
      linestream >>  key;
      if(key.find(cpu) == 0){
        
        while( linestream >> key) CPUs.emplace_back(key);
        break;
      }
    } 
  }
  return CPUs; }


/**
 * This function returns the total number of processes on the system
 * @param none
 * @return total number of processes on the system as int type
*/
int LinuxParser::TotalProcesses() { 

  string line, keyword;
  int totalProcesses = 0;
  ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(getline(filestream, line)){
      if(line.find("processes") == 0){
        istringstream linestream(line);
        linestream >> keyword >> totalProcesses;
      }

    }
  }
  // totalProcesses = std::stoi(value);
  return totalProcesses; }


/**
 * This function returns the number of processes actively running on the system
 * @param none
 * @return the number of processes as int type
*/
int LinuxParser::RunningProcesses() { 
  
  string line, keyword;
  int runningProc = 0;
  ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(getline(filestream, line)){
      if(line.find("procs_running") == 0){
        istringstream linestream(line);
        linestream >> keyword >> runningProc;
      }
    }
  }

  return runningProc; }


/*
    This function reads and returns the command associated with a process
    @param None
    @return  the command associated with a process as string type
*/ 
string LinuxParser::Command(int pid) { 
  string id, comnd;
  ifstream filestream(kProcDirectory + to_string(pid) +  kCmdlineFilename);
  if(filestream.is_open()) {
    getline(filestream, comnd);
  }
  return comnd; }


/*
    This function reads and returns the memory used by a process
    @param None
    @return  the memory used by a process as string type
*/ 
string LinuxParser::Ram(int pid) { 
  string line, key;
  long value;
  ifstream filestream(kProcDirectory + to_string(pid) +  kStatusFilename);
  if(filestream.is_open()) {
    while(getline(filestream, line)){
      if(line.find("VmSize:") == 0){
        istringstream linestream(line);
        linestream >> key >> value;
        break;
      }
    }
  }
  return to_string(value/ 1024); }


/*
    Return this process ID
    @param None
    @return  current process Uid
*/ 
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  ifstream filestream(kProcDirectory + to_string(pid)+ kStatusFilename);
  if(filestream.is_open()){
    while(getline(filestream, line)){
      if(line.find("Uid:")){
        istringstream linestream(line);
        linestream >> key >> value;
        break;
      }
    }
  }
  return value; }


/*
    Return the user (name) that generated this process
    @param None
    @return username of the current process_
*/ 
string LinuxParser::User(int pid) { 
  
  string line, name, x, uid;
  ifstream passwdStream(kPasswordPath);
  if(passwdStream.is_open()){
    
    while(getline(passwdStream, line)){
      replace(line.begin(), line.end(), ':', ' ');
      istringstream lineStream(line);
      lineStream >> name >> x >> uid;
      if(uid == LinuxParser::Uid(pid)) {
        return name;
        break;
      }
    }
  }
  return name; }
 

/**
 * This function Read and return the uptime of a process.
 * @param none
 * @return the number of seconds as long
*/
long LinuxParser::UpTime(int pid) { 
  
  string line, clock_ticks;
  ifstream filestream(kProcDirectory + to_string(pid) +  kStatFilename);
  if(filestream.is_open()){
    for(int i{0}; i < 22; i++) filestream >> clock_ticks;
   }
  return stol(clock_ticks)/sysconf(_SC_CLK_TCK); 
}


