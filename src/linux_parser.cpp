#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// return system info
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');  // convert ' ' to '_'
      std::replace(line.begin(), line.end(), '=', ' ');  // convert '=' to ' '
      std::replace(line.begin(), line.end(), '"', ' ');  // convert '"' to ' '
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);  // get string stream of each line and
                                          // " " as delimiter to get each token
    linestream >> os >> version >>
        kernel;  // store the 3rd token in string kernal
  }
  return kernel;
}

// return a vector of ints represents all pids
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

// REVIEW: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float MemTotal, MemFree, MemBuffers;
  float MemoryUtilization;
  string name, value, unit;
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> value >> unit;
      if (name == "MemTotal:") {
        MemTotal = stof(value);
      } else if (name == "MemFree:") {
        MemFree = stof(value);
      } else if (name == "Buffers:") {
        MemBuffers = stof(value);
      }
    }
  }
  filestream.close();
  MemoryUtilization = 1 - (MemFree / (MemTotal - MemBuffers));
  return MemoryUtilization;
}

// REVIEW: Read and return the system uptime in seconds
long LinuxParser::UpTime() {
  string UpTime;
  string IdleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);  // get string stream of each line and
                                          // " " as delimiter to get each token
    linestream >> UpTime >> IdleTime;
  }
  stream.close();
  return stol(UpTime);
}

// REVIEW: Read and return CPU utilization
// parse cpu utilization values from 'kStatFilename'
vector<string> LinuxParser::CpuUtilization() {
  string line;
  vector<string> CpuUtilization;
  string key, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    // only read the first row
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> key >> user >> nice >> system >> idle >> iowait >>
           irq >> softirq >> steal >> guest >> guest_nice) {
      CpuUtilization.push_back(user);
      CpuUtilization.push_back(nice);
      CpuUtilization.push_back(system);
      CpuUtilization.push_back(idle);
      CpuUtilization.push_back(iowait);
      CpuUtilization.push_back(irq);
      CpuUtilization.push_back(softirq);
      CpuUtilization.push_back(steal);
      CpuUtilization.push_back(guest);
      CpuUtilization.push_back(guest_nice);
    }
    return CpuUtilization;
  }
  filestream.close();
}

// REVIEW: Read and return the total number of jiffies for the system
// return the sum of active and idle jiffies from IdleJiffies() and
// ActiveJiffies()
long LinuxParser::Jiffies() {
  long Jiffies = 0;
  vector<string> CpuUtilization = LinuxParser::CpuUtilization();
  for (int i = 0; i < 8; i++) {
    Jiffies += stol(CpuUtilization[i]);
  }
  return Jiffies;
}

// REVIEW: Read and return the total number of idle jiffies for the system
// extract idle values from LinuxParser::CpuUtilization()
long LinuxParser::IdleJiffies() {
  long IdleJiffies = 0;
  vector<string> CpuUtilization = LinuxParser::CpuUtilization();
  for (int i = 3; i <= 4; i++) {
    IdleJiffies += stol(CpuUtilization[i]);
  }
  return IdleJiffies;
}

// REVIEW: Read and return the total number of active jiffies for the system
// extract the activa values from LinusParser::CpuUtilization()
long LinuxParser::ActiveJiffies() {
  long ActiveJiffies;
  ActiveJiffies = LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
  return ActiveJiffies;
}

// REVIEW: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long ActiveJiffies = 0;
  string value, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value) {
      if (i > 12 && i < 17) {
        ActiveJiffies += stol(value);
      }
      i++;
    }
  }
  filestream.close();
  return ActiveJiffies;
}

// REVIEW: Read and return the count of all processes in the system
int LinuxParser::TotalProcesses() {
  int TotalProcesses;
  string key, value, line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        TotalProcesses = stoi(value);
        return TotalProcesses;
      }
    }
  }
  filestream.close();
  return 0;
}

// REVIEW: Read and return the count of running processes in the system
int LinuxParser::RunningProcesses() {
  int RunningProcesses;
  string key, value, line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        RunningProcesses = stoi(value);
        return RunningProcesses;
      }
    }
  }
  filestream.close();
  return 0;
}

// REVIEW: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string Command;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, Command);
    return Command;
  }
  filestream.close();
  return "no command";
}

// REVIEW: Read and return the memory used by a process in MB
string LinuxParser::Ram(int pid) {
  string line, key, value;
  string Ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        Ram = to_string(stoi(value) / 1000);
        return Ram;
      }
    }
  }
  filestream.close();
  return "0";
}

// REVIEW: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  string Uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        Uid = value;
        return Uid;
      }
    }
  }
  filestream.close();
  return "no Uid";
}

// REVIEW: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, key, x, value;
  string User;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');  // convert ':' to ' '
    std::istringstream linestream(line);
    linestream >> key >> x >> value;
    if (value == LinuxParser::Uid(pid)) {
      User = key;
      return User;
    }
  }
  filestream.close();
  return string();
}

// REVIEW: Read and return the uptime of a process in seconds
long LinuxParser::UpTime(int pid) {
  string value, line;
  long UpTime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i = 0;
    while (linestream >> value) {
      if (i == 21) {
        UpTime = stol(value) / sysconf(_SC_CLK_TCK);
        return UpTime;
      }
      i++;
    }
  }
  filestream.close();
  return 0;
}