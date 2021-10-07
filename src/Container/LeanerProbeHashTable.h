//
// Created by 何智强 on 2021/10/3.
//

#ifndef MINIKV_LEANERPROBEHASHTABLE_H
#define MINIKV_LEANERPROBEHASHTABLE_H

/// Cant allocate slots for all possible keys, so we have to deal with hash collision.
/// LeanerProbeHashTable use fixed size of hash table, if we run out of all free slots, allocate a twice larger array.
/// When collision happens, new entry will be inserted into next free slots after position where it should be.

#include "src/Storage/BufferPool/BufferPoolManager.h"

namespace miniKV {
    template<typename KeyType, typename ValueType>
    class LeanerProbeHashTable {
    public:
        explicit LeanerProbeHashTable (
                std::shared_ptr<BufferPoolManager> bufferPoolManager_,
                size_t numBuckets_
        ) : bufferPoolManager(bufferPoolManager_), num_buckets(numBuckets_) {
            auto header_page = bufferPoolManager->NewPage();
            header_page_id = header_page->GetPageId();
            hashFunc = std::hash<KeyType>{};
        };

        bool GetValue(const KeyType& key, ValueType& value);
        bool Insert(const KeyType& key, const ValueType& value);
        bool Remove(const KeyType& key);
        void Resize(size_t size);
        size_t GetSize() const;


    private:
        page_id_t header_page_id;

        std::shared_ptr<BufferPoolManager> bufferPoolManager;
        size_t num_buckets;
        std::function<KeyType> hashFunc;
    };
}

#endif //MINIKV_LEANERPROBEHASHTABLE_H
