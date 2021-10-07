//
// Created by 何智强 on 2021/10/4.
//

#ifndef MINIKV_HASHTABLEPAGE_H
#define MINIKV_HASHTABLEPAGE_H
#include "src/Common/Config.h"

namespace miniKV {


#define HASH_TABLE_BLOCK_TYPE HashTableBlockPage<KeyType, ValueType, KeyComparator>


template<typename KeyType, typename ValueType>
class HashTablePage {
public:
    using MappingType = std::pair<KeyType, ValueType>;

    /** BLOCK_ARRAY_SIZE is the number of (key, value) pairs that can be stored in   * a block page. It is an approximate
    * calculation based on the size of MappingType (which is a std::pair of KeyType and ValueType). For each key/value
    * pair, we need two additional bits for occupied_ and readable_. 4 * PAGE_SIZE / (4 * sizeof (MappingType) + 1) =
    * PAGE_SIZE/(sizeof (MappingType) + 0.25) because 0.25 bytes = 2 bits is the space required to maintain the occupied
    * and readable flags for a key value pair.*/
    #define BLOCK_ARRAY_SIZE (4 * PAGE_SIZE / (4 * sizeof(MappingType) + 1))

    HashTablePage();

    KeyType KeyAt(size_t slot_offset) const;
    ValueType ValueAt(size_t slot_offset) const;
    bool Insert(size_t slot_offset, const KeyType& key, const ValueType& value);
    void Remove(size_t slot_offset);

   /**
   * Returns whether or not an index is occupied (key/value pair or tombstone)
   *
   * @param slot_offset index to look at
   * @return true if the index is occupied, false otherwise
   */
    bool IsOccupied(size_t slot_offset) const;

    /**
   * Returns whether or not an index is readable (valid key/value pair)
   *
   * @param slot_offset index to look at
   * @return true if the index is readable, false otherwise
   */
    bool IsReadable(size_t slot_offset) const;

    inline size_t Size() const { return size; }

    inline page_id_t GetPageID() const { return page_id; }

    inline page_id_t GetNextPageId() const { return next; }

    inline bool IsFull() const {
        assert(size <= BLOCK_ARRAY_SIZE);
        return size == BLOCK_ARRAY_SIZE;
    }

    void SetPageID(page_id_t page_id_) { page_id = page_id_; }

    void SetNextPageID(page_id_t next_) { next = next_; }

private:
    page_id_t page_id;
    page_id_t next;
    size_t size;

    std::atomic_char occupied[(BLOCK_ARRAY_SIZE - 1) / 8 + 1];
    std::atomic_char readable[(BLOCK_ARRAY_SIZE - 1) / 8 + 1];
    MappingType array[0];
};


}




#endif //MINIKV_HASHTABLEPAGE_H
