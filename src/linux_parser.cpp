#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include "linux_parser.h"
#include <iostream>
#include <map>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::endl;

// DONE: 
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
 
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
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

// DONE: 
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> os >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::ifstream file(kProcDirectory+kMeminfoFilename);
  std::string line;
  std::vector<float> memVec;
  std::string name, val;
  if (file.is_open()){

    for (int i=0; i<4; ++i){
      std::getline(file, line);
      std::stringstream ss(line);
      ss >> name >> val;
      memVec.push_back(std::atof(val.c_str()));
    }
  }

  float utilization = 1-memVec[2]/memVec[0];
  return utilization;
}

// Done
long LinuxParser::UpTime() { 
  std::ifstream file(kProcDirectory+kUptimeFilename);
  std::string line;

  float uptime;
  float total=0;
  if (file.is_open()){
    std::getline(file, line);
    std::stringstream ss(line);
    while(ss >> uptime){
      total += uptime;
      break;
    }
  }
  return (long)total;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
std::vector<float> LinuxParser::CpuUtilization() {
    std::ifstream stream(kProcDirectory  + "/" + kStatFilename);
    std::string cpu;
    std::string line;
    std::string code;
    std::vector<float> stat;
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> cpu;

      while (linestream >> code){
        stat.push_back(std::stof(code));
      }
    }
    return stat;
 }

float LinuxParser::CpuPidUtilization(int pid){
    std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + "/" + kStatFilename);
    std::string line;
    std::string code;
    std::vector<std::string> status;
    if (stream.is_open()){
      std::getline(stream, line);
      std::istringstream linestream(line);
      while (linestream >> code){
        status.push_back(code);
      }
    }

    int utime = std::stol(status[LinuxParser::CPUPidStates::kUtime_]);
    int  stime = std::stol(status[LinuxParser::CPUPidStates::kStime_]);
    int cutime = std::stol(status[LinuxParser::CPUPidStates::kCUtime_]);
    int cstime = std::stol(status[LinuxParser::CPUPidStates::kCStime_]);
    int starttime = std::stol(status[LinuxParser::CPUPidStates::upTime_]);

    int upTime = UpTime();
    
    int totalTime = utime + stime + cutime + cstime;
    float Hertz = sysconf(_SC_CLK_TCK);
    int seconds = upTime - (starttime / Hertz );


    float cpuUsage =  ((totalTime / Hertz) / (seconds+0.1));

    if (cpuUsage < 0)
        cpuUsage = 0;
    return cpuUsage;
}
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream stream(kProcDirectory + kStatFilename);

  std::string line;
  std::string key;
  int numProcess;
  while (getline(stream, line)){
     std::istringstream ss(line);
     ss >> key;
     if (key=="processes"){
       ss >> numProcess;
       break;
     }
  }
  return numProcess;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    std::ifstream stream(kProcDirectory + kStatFilename);

  std::string line;
  std::string key;
  int numProcess;
  while (getline(stream, line)){
     std::istringstream ss(line);
     ss >> key;
     if (key=="procs_running"){
       ss >> numProcess;
       break;
     }
  }
  return numProcess;
}

// TODO: 
string LinuxParser::Command(int pid) { 
    std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + "/" + kCmdlineFilename);
    std::string cmd;
    if (stream.is_open()){
      std::getline(stream, cmd);
    }
    return cmd;
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + "/" + kStatusFilename);
  std::string key;
  std::string val;
  std::string line;
  if (stream.is_open()){     
    while(std::getline(stream, line)){
      std::istringstream ss(line);
      ss >> key >> val;
      if (key=="VmSize:"){
        return std::to_string(atoi(val.c_str())/1000);
      }
    }
  } 
  return "";
 }

// TODO: 

int LinuxParser::Uid(int pid) { 
  std::ifstream stream(kProcDirectory+"/" +std::to_string(pid) + kStatusFilename);
  std::string Uid;
  int id;
  std::string line;
  if (stream.is_open()){
      while (std::getline(stream, line)) {
      std::istringstream ss(line);
      ss >> Uid >> id;
      if (Uid == "Uid:"){
        return id;
      }
    }
  }
  return -1;
 }

// TODO: Read and return the user associated with a process

std::map<int, std::string> LinuxParser::UserDict() { 
    std::ifstream stream(kPasswordPath);
    std::map<int, std::string> userDict;
    std::string line;
    std::string name;
    std::string other;
    int uid;

    if (stream.is_open()) {
      while (std::getline(stream, line)){
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream ss(line);
        ss >> name >> other >> uid;
        userDict[uid] = name;
      }
    }

    userDict[-1] = "none";
  return userDict;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long  LinuxParser::UpTime(int pid){
    std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
    std::string line;
    std::string code;
    std::vector<std::string> status;
    if (stream.is_open()){
      std::getline(stream, line);
      std::istringstream linestream(line);
      while (linestream >> code){
        status.push_back(code);
      }
    }

    long ticks = std::stol(status[LinuxParser::CPUPidStates::upTime_]);
    return ticks;
  }