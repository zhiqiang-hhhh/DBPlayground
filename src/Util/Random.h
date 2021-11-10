//
// Created by 何智强 on 2021/10/22.
//

#ifndef MINIKV_RANDOM_H
#define MINIKV_RANDOM_H

#include <cstdint>
#include <vector>

class Random {
 public:
  int32_t GetValue();
  std::vector<int32_t> GetSequence();
};

#endif  // MINIKV_RANDOM_H
