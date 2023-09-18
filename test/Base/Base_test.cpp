//
// Created by 何智强 on 2021/11/15.
//

#include <gtest/gtest.h>

#include <thread>
#include <vector>

#include "Base/BoundedQueue.h"

template <typename... Args>
void LaunchParallelThreads(std::vector<std::thread> &threads, size_t num_threads, Args &&...args) {
  for (size_t thread_itr = 0; thread_itr < num_threads; ++thread_itr) {
    threads.push_back(std::thread(std::forward<Args>(args)...));
  }
}

void WaitThreadFinish(std::vector<std::thread> &threads) {
  for (auto &thread : threads) {
    if (thread.joinable()) thread.join();
  }
}

void PushHelper(const std::vector<int32_t> &keys, BoundedQueue<int32_t> &queue) {
  for (const auto &key : keys) {
    queue.PushBack(key);
  }
}

TEST(BaseTest, BoundedQueuePushTest) {
  const size_t queue_size = 30;
  const size_t thread_num = 5;
  std::shared_ptr<BoundedQueue<int32_t>> queue_ptr = std::make_shared<BoundedQueue<int32_t>>(100);
  BoundedQueue<int32_t> queue(queue_size);
  std::vector<int32_t> keys;

  for (int32_t key = 0; key < queue_size; ++key) {
    keys.emplace_back(key);
  }

  std::vector<std::thread> threads;
  LaunchParallelThreads(threads, thread_num, PushHelper, keys, std::ref(queue));

  std::vector<int32_t> values;
  while (true) {
    int32_t value;
    queue.Pop(value);
    values.emplace_back(value);

    if (values.size() == queue_size * thread_num) break;
  }

  WaitThreadFinish(threads);

  ASSERT_EQ(values.size(), queue_size * thread_num);
}