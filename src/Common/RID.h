//
// Created by 何智强 on 2021/10/3.
//

#ifndef MINIKV_RID_H
#define MINIKV_RID_H

#include <string>

#include "Config.h"

namespace miniKV {

class RID {
 public:
  /** The default constructor creates an invalid RID! */
  RID() = default;

  /**
   * Creates a new Record Identifier for the given page identifier and slot number.
   * @param page_id page identifier
   * @param slot_num slot number
   */
  RID(page_id_t page_id, uint32_t slot_num) : page_id_(page_id), slot_num_(slot_num) {}

  explicit RID(int64_t rid) : page_id_(static_cast<page_id_t>(rid >> 32)), slot_num_(static_cast<uint32_t>(rid)) {}

  inline int64_t Get() const { return (static_cast<int64_t>(page_id_)) << 32 | slot_num_; }

  inline page_id_t GetPageId() const { return page_id_; }

  inline uint32_t GetSlotNum() const { return slot_num_; }

  inline void Set(page_id_t page_id, uint32_t slot_num) {
    page_id_ = page_id;
    slot_num_ = slot_num;
  }

  inline std::string ToString() const {
    std::stringstream os;
    os << "page_id: " << page_id_;
    os << " slot_num: " << slot_num_ << "\n";

    return os.str();
  }

  friend std::ostream &operator<<(std::ostream &os, const RID &rid) {
    os << rid.ToString();
    return os;
  }

  bool operator==(const RID &other) const { return page_id_ == other.page_id_ && slot_num_ == other.slot_num_; }

 private:
  page_id_t page_id_{INVALID_PAGE_ID};
  uint32_t slot_num_{0};  // logical offset from 0, 1...
};

}  // namespace miniKV

#endif  // MINIKV_RID_H
