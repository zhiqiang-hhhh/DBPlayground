//
// Created by 何智强 on 2021/10/2.
//

#ifndef MINIKV_MINIKV_H
#define MINIKV_MINIKV_H

#include <vector>
#include "src/Common/Config.h"
#include "src/Storage/BufferPool/BufferPoolManager.h"
#include "src/Container/BPlusTree.h"

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
    BPlusTree<key_t, value_t > container;
};
    
}



#endif //MINIKV_MINIKV_H
