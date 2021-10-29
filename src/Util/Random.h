//
// Created by 何智强 on 2021/10/22.
//

#ifndef MINIKV_RANDOM_H
#define MINIKV_RANDOM_H

class Random {
 public:
  Int32_t GetValue();
  std::vector<Int32_t> GetSequence();
};

#endif  // MINIKV_RANDOM_H
