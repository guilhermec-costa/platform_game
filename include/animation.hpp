#pragma once

#include <string>
#include <vector>

struct Animation {
  std::string        name;
  int                start_frame;
  int                end_frame;
  float              default_frame_time;
  bool               loop = true;
  std::vector<float> frame_times;
};