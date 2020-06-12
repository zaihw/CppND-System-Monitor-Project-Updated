#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// constructor
Process::Process(int pid) { Process::pid_ = pid; }

// REVIEW: Return this process's ID
int Process::Pid() { return pid_; }

// REVIEW: Return this process's CPU utilization
float Process::CpuUtilization() {
  int hertz = sysconf(_SC_CLK_TCK);
  float CpuUtilization = 0.0;

  // string key, line, value;
  // vector<string> data;
  // std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid_) +
  //                          LinuxParser::kStatFilename);
  // if (filestream.is_open()) {
  //   std::getline(filestream, line);
  //   std::istringstream linestream(line);
  //   for (int i = 0; i <= 21; i++) {
  //     linestream >> value;
  //     data.push_back(value);
  //   }
  // }

  // long seconds = LinuxParser::UpTime() - stof(data[21]) / hertz;
  // long totaltime = stof(data[13]) + stof(data[14]) + stof(data[15]) + stof(data[16]);

  long totaltime = LinuxParser::ActiveJiffies(pid_);
  long seconds = LinuxParser::UpTime(pid_);
  CpuUtilization = (((float)totaltime / (float)hertz) / (float)seconds);
  return CpuUtilization;
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
  return stol(LinuxParser::Ram(pid_)) > stol(LinuxParser::Ram(a.pid_));
}