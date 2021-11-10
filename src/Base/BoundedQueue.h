//
// Created by 何智强 on 2021/10/29.
//

#ifndef MINIKV_BOUNDEDQUEUE_H
#define MINIKV_BOUNDEDQUEUE_H

#include <list>


template<typename T>
class BoundedQueue {
public:
    BoundedQueue() = default;
    BoundedQueue(const size_t& size) : capacity(size) {
        queue.resize(size);
    }

    bool PushBack(const T& element);
    bool Pop(T& element_ret);
    bool IsFull() const;

private:
    std::list<T> queue;
    size_t capacity;
};

#endif //MINIKV_BOUNDEDQUEUE_H
