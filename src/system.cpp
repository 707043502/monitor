#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
     std::map<int, std::string> userDict = LinuxParser::UserDict();

    std::set<int> liveSet_;
    std::vector<int> pids = LinuxParser::Pids();
    for (auto pid: pids){
        liveSet_.insert(pid);

        if (processSet_.find(pid) != processSet_.end()){
            continue;
        }        

        int uid = LinuxParser::Uid(pid);
        Process process(pid, uid, userDict);
        processes_.push_back(process);
        processSet_.insert(pid);
    }

    for (auto iter=processes_.begin(); iter!= processes_.end(); ){
        if (liveSet_.find(iter->Pid()) != liveSet_.end()) {
            iter->UpdateCPUUse();
            iter++;
            continue;
        }
        

        iter = processes_.erase(iter);
       
    }

    std::sort(processes_.begin(), processes_.end());

    return processes_;
 }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {return LinuxParser::Kernel();}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses();}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }