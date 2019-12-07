#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
  void SetTickGain(float);
  float GetTickGain();
 private:
    float tickGain_ = 1000;
    std::vector<float> status_={};
};

#endif