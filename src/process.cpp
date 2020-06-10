#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stol;
using std::string;
using std::to_string;
using std::vector;

// constructor
Process::Process(int pid): pid_(pid){}

// REVIEW: Return this process's ID
int Process::Pid() { return pid_; }

// REVIEW: Return this process's CPU utilization
float Process::CpuUtilization() {
  long uptime = LinuxParser::UpTime();
  int hertz = sysconf(_SC_CLK_TCK);
  long starttime = LinuxParser::UpTime(pid_);
  long totaltime = LinuxParser::ActiveJiffies(pid_);
  long CpuUtilization;
  CpuUtilization = 100 * ((totaltime / hertz) / (uptime - (starttime / hertz)));
  return (float)CpuUtilization;
}

// REVIEW: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// REVIEW: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// REVIEW: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// REVIEW: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// REVIEW: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return stol(LinuxParser::Ram(pid_)) > stol(LinuxParser::Ram(pid_));
}