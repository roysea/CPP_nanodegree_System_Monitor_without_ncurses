#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../include/format.h"
#include "../include/system.h"

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::setw;
using std::left;
using Format::ProgressBar;
using Format::ElapsedTime;


void DisplaySystem(System& system);
void DisplayProcesses(std::vector<Process>& processes);

int main() {
  System system;
  string s(100, '*'), s1(42, ' ');
  cout << s << "\n" << "*" << s1 << "SYSTEM MONITOR" << s1 << "*" << "\n" << s << endl;
  DisplaySystem(system);
  DisplayProcesses(system.Processes());
  return 0;
}

void DisplaySystem(System& system) {
  cout << "OS: " + system.OperatingSystem() << endl;
  cout << "Kernel: " + system.Kernel() << endl;

  cout << "CPU:  ";
  cout << ProgressBar(system.Cpu().Utilization("cpu")) << endl;

  int number_of_cpus = system.NumberOfCPUs();
  if(number_of_cpus > 0){
    for(int i{0}; i < number_of_cpus; i++){

      string CPU_name = "CPU" + to_string(i) + ": ";
      cout << CPU_name;
      cout << ProgressBar(system.Cpu().Utilization("cpu" + to_string(i))) << endl;
    }
  }

  cout << "Memory:  ";
  cout << ProgressBar(system.MemoryUtilization()) << endl;
  cout << "Total Processes:   " + to_string(system.TotalProcesses()) << endl;
  cout << "Running Processes: " + to_string(system.RunningProcesses()) << endl;
  cout << "Up Time:           " + ElapsedTime(system.UpTime()) << endl;
  string s(97, '*'), s1(39, ' ');
  cout << s << "\n" << "*" << s1 << "PROCESSES MONITOR" << s1 << "*" << "\n" << s << endl;
}

void DisplayProcesses(std::vector<Process>& processes) {
  
  cout << left;
  
  // int const num_processes = processes.size();
  cout << setw(5) << "PID"<< setw(7)  << "USER" << setw(7)  <<  "CPU%" << setw(15) <<  "RAM[MB]";
  cout << setw(10) <<  "TIME+" << setw(55)<<  "COMMAND"  << "\n" << endl;
  
  for (int i{1}; i < 8; ++i) {

    cout << setw(5) <<  to_string(processes[i].Pid());
    if( processes[i].User() == "fwupd-refresh") cout << setw(7) << "User";
    else cout << processes[i].User();
    float cpu = processes[i].CpuUtilization() * 100;

    cout << setw(7) <<  to_string(cpu).substr(0, 4);
    cout << setw(15) <<   processes[i].Ram();
    cout << setw(10) <<  ElapsedTime(processes[i].UpTime());
    cout << setw(85)<<  processes[i].Command().substr(0, 50);
    cout << endl;
  }
  
}
