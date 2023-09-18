//
// Created by 何智强 on 2021/10/2.
//

#ifndef MINIKV_MINIKV_H
#define MINIKV_MINIKV_H

#include <vector>

#include "Common/Config.h"
#include "Container/BPlusTree.h"
#include "Storage/BufferPool/BufferPoolManager.h"

namespace miniKV {

class MiniKV {
 public:
  MiniKV();

  bool insert(key_t k, value_t v);
  bool update(key_t k, value_t v);
  bool remove(key_t);
  value_t get(key_t);
  values range(key_t);

 private:
  std::shared_ptr<DiskManager> disk_manager;
  std::shared_ptr<BufferPoolManager> bpm;
  BPlusTree<key_t, value_t> container;
};

}  // namespace miniKV

#endif  // MINIKV_MINIKV_H
