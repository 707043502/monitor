#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <map>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_ = 1,
  kSystem_ = 2,
  kIdle_ = 3 ,
  kIOwait_ = 4 ,
  kIRQ_ = 5,
  kSoftIRQ_= 6,
  kSteal_ = 7,
  kGuest_ = 8,
  kGuestNice_ = 9
};
std::vector<float> CpuUtilization();

enum CPUPidStates {
  kUtime_ = 13,
  kStime_ = 14,
  kCUtime_ = 15,
  kCStime_ = 16,
  upTime_ = 21
};

float  CpuPidUtilization(int);
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
int Uid(int pid);
std::map<int,std::string> UserDict();
long int UpTime(int pid);
};  // namespace LinuxParser

#endif