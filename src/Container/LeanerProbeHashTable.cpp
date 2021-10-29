//
// Created by 何智强 on 2021/10/3.
//

#include "LeanerProbeHashTable.h"
#include "src/Storage/Page/HashTableHeaderPage.h"
#include "src/Storage/Page/HashTablePage.h"

namespace miniKV {

template <typename KeyType, typename ValueType>
bool LeanerProbeHashTable<KeyType, ValueType>::GetValue(const KeyType &key, ValueType &value) {
  /// 1. Calculate bucket index
  /// 2. Fetch block page
  /// 3. Return value
  std::size_t hash_value = hashFunc(key);
  std::size_t bucket_num = hash_value % num_buckets;
  std::size_t slot_num = hash_value - bucket_num * miniKV::PAGE_SIZE;
  std::shared_ptr<Page> page_ptr = bufferPoolManager->FetchPage(bucket_num);
  if (!page_ptr) {
  }
  page_ptr->RLatch();
  std::shared_ptr<HashTablePage<KeyType, ValueType>> table_page(
      reinterpret_cast<HashTablePage<KeyType, ValueType> *>(page_ptr->GetData()));
  value = table_page->ValueAt(slot_num);
  return true;
}

template class LeanerProbeHashTable<miniKV::key_t, miniKV::value_t>;
}  // namespace miniKV
