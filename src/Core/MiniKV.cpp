//
// Created by 何智强 on 2021/10/2.
//

#include "MiniKV.h"

namespace miniKV {

MiniKV::MiniKV()
    : disk_manager(new DiskManager("miniKV.db")),
      bpm(new BufferPoolManager(BUFFER_POOL_SIZE, disk_manager)),
      container(bpm) {}

value_t MiniKV::get(key_t key) {
  value_t value;
  if (container.GetValue(key, value)) return value;

  return -1;
}

bool MiniKV::insert(key_t key, value_t value) { return container.Insert(key, value); }

bool MiniKV::update(key_t key, value_t value) { return container.Insert(key, value); }

bool MiniKV::remove(key_t key) {
  container.Remove(key);
  return true;
}

}  // namespace miniKV
