//
// Created by 何智强 on 2021/10/17.
//

#include <random>
#include <thread>
#include <vector>

#include "gtest/gtest.h"
#include "src/Container/BPlusTree.h"
#include "src/Storage/BufferPool/BufferPoolManager.h"
#include "src/Storage/Disk/DiskManager.h"

namespace miniKV {

const size_t NUM_TRIES = 1;

template <typename... Args>
void LaunchParallelTest(uint32_t num_threads, Args &&...args) {
  std::vector<std::thread> threads;

  for (uint32_t thread_itr = 0; thread_itr < num_threads; ++thread_itr) {
    threads.push_back(std::thread(args..., thread_itr));
  }

  for (auto &thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void InsertHelper(std::shared_ptr<BPlusTree<key_t, value_t>> tree, const std::vector<key_t> keys, uint64_t tid) {
  Transaction *transaction = new Transaction(tid);
  for (const auto &key : keys) {
    value_t value = key & 0xFFFFFFFF;
    tree->Insert(key, value, transaction);
  }
  delete transaction;
}

void DeleteHelper(std::shared_ptr<BPlusTree<key_t, value_t>> tree, const std::vector<key_t> keys, uint64_t tid) {
  Transaction *transaction = new Transaction(tid);
  for (const auto &key : keys) {
    tree->Remove(key, transaction);
  }
  delete transaction;
}

TEST(CoreTest, DISABLED_Concurrent_Insert_Test) {
  for (size_t iter = 0; iter < NUM_TRIES; ++iter) {
    auto disk_manager = std::make_shared<DiskManager>("test.db");
    auto buffer_pool_manager = std::make_shared<BufferPoolManager>(50, disk_manager);
    auto container = std::make_shared<BPlusTree<key_t, value_t>>(buffer_pool_manager);

    std::vector<key_t> keys;
    size_t NUM_KEYS = 200000;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<key_t> dist;
    for (size_t iter = 0; iter < NUM_KEYS; ++iter) {
      keys.push_back(dist(mt));
    }

    //        auto insert_helper = [&]() {
    //            for (size_t iter = 0; iter < NUM_KEYS; ++iter) {
    //                container->Insert(keys[iter], keys[iter] & 0xFFFFFFFF);
    //            }
    //        };

    size_t NUM_THREADS = 1;
    for (size_t iter = 0; iter < NUM_KEYS;) {
      const std::vector<key_t> keys_interval{keys.begin() + iter, keys.begin() + iter + NUM_KEYS / NUM_THREADS};

      LaunchParallelTest(1, InsertHelper, container, keys_interval);

      iter += NUM_KEYS / NUM_THREADS;
    }
    //        EXPECT_EQ(container->GetSize(), NUM_KEYS);

    // LaunchParallelTest(1, InsertHelper, container, keys);

    for (size_t iter = 0; iter < NUM_KEYS; ++iter) {
      key_t key = keys[iter];
      value_t value;
      container->GetValue(key, value);
      EXPECT_EQ((value_t)(key & 0xFFFFFFFF), value);
    }

    remove("test.db");
  }
}

TEST(CoreTest, Concurrent_Remove_Test) {
  for (size_t iter = 0; iter < NUM_TRIES; ++iter) {
    auto disk_manager = std::make_shared<DiskManager>("test.db");
    auto buffer_pool_manager = std::make_shared<BufferPoolManager>(50, disk_manager);
    auto container = std::make_shared<BPlusTree<key_t, value_t>>(buffer_pool_manager);

    std::vector<key_t> keys;
    size_t NUM_KEYS = 2000;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<key_t> dist;
    for (size_t iter = 0; iter < NUM_KEYS; ++iter) {
      keys.push_back(dist(mt));
    }

    size_t NUM_THREADS = 10;

    InsertHelper(container, keys, 1);

    const size_t NUM_PER_THREAD = 10;
    for (size_t iter = 0; iter < NUM_THREADS; ++iter) {
      std::vector<key_t> key_interval{keys.begin() + iter, keys.begin() + iter + NUM_PER_THREAD};
      LaunchParallelTest(1, DeleteHelper, container, key_interval);
    }

    for (size_t iter = 0; iter < NUM_KEYS; ++iter) {
      key_t key = keys[iter];
      value_t value;
      bool found = container->GetValue(key, value);
      if (iter < (NUM_PER_THREAD * NUM_THREADS))
        EXPECT_EQ(found, false);
      else {
        EXPECT_EQ(found, true);
        EXPECT_EQ((value_t)(key & 0xFFFFFFFF), value);
      }
    }

    remove("test.db");
  }
}
}  // namespace miniKV
