#include "processor.h"

#include <unistd.h>

#include "linux_parser.h"

// REVIEW: Return the aggregate CPU utilization
float Processor::Utilization() {
  long prev_totaljiffies_ = LinuxParser::Jiffies();
  long prev_activejiffies_ = LinuxParser::ActiveJiffies();

  usleep(500000);

  long totaljiffies_ = LinuxParser::Jiffies();
  long activejiffies_ = LinuxParser::ActiveJiffies();

  float Utilization;
  Utilization = (float)(activejiffies_ - prev_activejiffies_) /
                (totaljiffies_ - prev_totaljiffies_);
  return Utilization;
}