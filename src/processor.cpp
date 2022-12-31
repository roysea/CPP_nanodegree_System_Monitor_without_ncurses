#include "../include/processor.h"
#include "../include/linux_parser.h"
#include <string>


/*
    This function returns the CPU utilization for a specified cpu
    @param None
    @return float
*/ 
float Processor::Utilization(std::string cpu) { 
   // ratio of active time = nonIdleTime / (idleTime + nonIdleTime);
  
    return (float)LinuxParser::ActiveJiffies(cpu) / LinuxParser::Jiffies(cpu); }