#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <processor.h>
#include <map>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, int uid,  std::map<int, std::string>userDict);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();    
  float UseView();              // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  void UpdateCPUUse();
  float cpuUse_ = -1;
  // TODO: Declare any necessary private members
 private:
    int pid_;
    int uid_;
    
     std::map<int, std::string> userDict_;
};

#endif