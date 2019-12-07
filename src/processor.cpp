#include <vector>
#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
Processor::Processor(){
    status_ = std::vector<float>{};
}

void Processor::SetTickGain(float tickGain){
    tickGain_=tickGain;
}

float Processor::GetTickGain(){
    return tickGain_;
}

float Processor::Utilization() {
    if (status_.size() < 1){
        status_ = LinuxParser::CpuUtilization();
        return 0.0;
    }

    std::vector<float> status = LinuxParser::CpuUtilization();
    float idleNow= status[LinuxParser::CPUStates::kIdle_];
    float ioWaitNow = status[LinuxParser::CPUStates::kIOwait_];

    float total = 0.0;
    for (size_t i=0; i<status_.size(); ++i) {
        total += (status[i] - status_[i]);
    }

    SetTickGain(total);

    int totalUseNow = idleNow + ioWaitNow;
    int totalUsePrev = status_[LinuxParser::CPUStates::kIdle_] + status_[LinuxParser::CPUStates::kIOwait_];

    status_ = status;

    return 1- (totalUseNow - totalUsePrev) / total;
 }
 