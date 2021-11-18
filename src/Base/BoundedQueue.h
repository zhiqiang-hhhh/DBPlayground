//
// Created by 何智强 on 2021/10/29.
//

#ifndef MINIKV_BOUNDEDQUEUE_H
#define MINIKV_BOUNDEDQUEUE_H

#include <iostream>
#include <list>

template <typename T>
class BoundedQueue {
 public:
  BoundedQueue() = default;
  BoundedQueue(const BoundedQueue &) = delete;
  BoundedQueue(const size_t &size) : capacity(size) {}

  bool PushBack(const T &element) {
    std::unique_lock<std::mutex> queue_lock(queue_mutex);
    while (queue.size() >= capacity) queue_cond.wait(queue_lock);
    queue.emplace_back(element);
    queue_cond.notify_one();
    return true;
  }

  bool Pop(T &element_ret) {
    std::unique_lock<std::mutex> queue_lock(queue_mutex);
    while (queue.empty()) queue_cond.wait(queue_lock);

    queue.pop_back();
    queue_cond.notify_one();
    return true;
  }

  bool IsFull() {
    std::unique_lock<std::mutex> queue_lock(queue_mutex);
    return queue.size() >= capacity;
  }

 private:
  std::list<T> queue;
  size_t capacity;
  std::mutex queue_mutex;
  std::condition_variable queue_cond;
};

#endif  // MINIKV_BOUNDEDQUEUE_H
