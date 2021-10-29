//
// Created by 何智强 on 2021/10/4.
//

#ifndef MINIKV_IPAGE_H
#define MINIKV_IPAGE_H

#include <cstring>
#include <iostream>

#include "src/Base/ReaderWriterLatch.h"
#include "src/Common/Config.h"

namespace miniKV {

class Page {
  friend class BufferPoolManager;

 public:
  /** Constructor. Zeros out the page data. */
  Page() { ResetMemory(); }

  /** Default destructor. */
  ~Page() = default;

  /** @return the actual data contained within this page */
  inline char *GetData() { return data; }

  /** @return the page id of this page */
  inline page_id_t GetPageId() { return page_id; }

  /** @return the pin count of this page */
  inline int GetPinCount() { return pin_count; }

  /** @return true if the page in memory has been modified from the page on disk, false otherwise */
  inline bool IsDirty() { return is_dirty; }

  /** Acquire the page write latch. */
  inline void WLatch() { rwlatch.WLock(); }

  /** Release the page write latch. */
  inline void WUnlatch() { rwlatch.WUnlock(); }

  /** Acquire the page read latch. */
  inline void RLatch() { rwlatch.RLock(); }

  /** Release the page read latch. */
  inline void RUnlatch() { rwlatch.RUnlock(); }

 protected:
  static_assert(sizeof(page_id_t) == 4);

  static constexpr size_t SIZE_PAGE_HEADER = 8;
  static constexpr size_t OFFSET_PAGE_START = 0;
  static constexpr size_t OFFSET_LSN = 4;

 private:
  inline void ResetMemory() { memset(data, OFFSET_PAGE_START, PAGE_SIZE + 16); }

  char data[PAGE_SIZE];
  page_id_t page_id = -1;
  int pin_count = 0;
  bool is_dirty = false;
  ReaderWriterLatch rwlatch;
};

}  // namespace miniKV
#endif  // MINIKV_IPAGE_H
