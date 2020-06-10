#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long prev_totaljiffies_;
  long prev_activejiffies_;
  long totaljiffies_;
  long activejiffies_;
};

#endif