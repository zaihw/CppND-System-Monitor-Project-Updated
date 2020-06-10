
#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  vector<Process> processes_ = {};
  for (auto pid : pids) {
    processes_.push_back(Process(pid));
  }
  return processes_;
}

// REVIEW: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// REVIEW: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// REVIEW: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// REVIEW: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// REVIEW: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// REVIEW: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }