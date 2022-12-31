#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <iostream>

#include "../include/format.h"
#include "../include/system.h"

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using Format::ProgressBar;
using Format::ElapsedTime;

void DisplaySystem(System& system);
void DisplayProcesses(std::vector<Process>& processes);

int main() {
  System system;
  DisplaySystem(system);
  DisplayProcesses(system.Processes());
  return 0;
}

void DisplaySystem(System& system) {
  cout << "OS: " + system.OperatingSystem() << endl;
  cout << "Kernel: " + system.Kernel() << endl;

  cout << "CPU: ";
  cout << ProgressBar(system.Cpu().Utilization("cpu")) << endl;

  int number_of_cpus = system.NumberOfCPUs();
  if(number_of_cpus > 0){
    for(int i{0}; i < number_of_cpus; i++){

      string CPU_name = "CPU" + to_string(i) + ": ";
      cout << CPU_name;
      cout << ProgressBar(system.Cpu().Utilization("cpu" + to_string(i))) << endl;
    }
  }

  cout << "Memory: ";
  cout << ProgressBar(system.MemoryUtilization()) << endl;
  cout << "Total Processes: " + to_string(system.TotalProcesses()) << endl;
  cout << "Running Processes: " + to_string(system.RunningProcesses()) << endl;
  cout << "Up Time: " + ElapsedTime(system.UpTime()) << endl;
  cout << "----------------------------------------------------------------------" << endl;
}

void DisplayProcesses(std::vector<Process>& processes) {
  int const num_processes = processes.size();
  cout << "PID" << " " << "USER" << " " <<  "CPU[%%]" << " " <<  "RAM[MB]" << " " <<  "TIME+" << " " <<  "COMMAND"  << endl;
  for (int i = 0; i < num_processes; ++i) {
    cout << " "<<  to_string(processes[i].Pid());
    if( processes[i].User() == "fwupd-refresh") cout << "  User";
    else cout << processes[i].User();
    float cpu = processes[i].CpuUtilization() * 100;
    cout << " " <<  to_string(cpu).substr(0, 4);
    cout << " " <<   processes[i].Ram();
    cout << " " <<  ElapsedTime(processes[i].UpTime()) ;
    cout <<  processes[i].Command().substr(0, 100);
    cout << endl;
  }
  
}
