#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  //based on a stack overflow post
  float cur_Idle{0};
  float prevIdle{0};
  float cur_NonIdle{0}; //active jiffies
  float prevNonIdle{0};
  float cur_Total{0};
  float prevTotal{0};
  float Tot_delta{0};
  float Idle_delta{0};
};

#endif