//
// Created by 何智强 on 2021/10/5.
//

#ifndef MINIKV_HASHTABLEHEADERPAGE_H
#define MINIKV_HASHTABLEHEADERPAGE_H

#include "src/Common/Config.h"

namespace miniKV {
class HashTableHeaderPage {
 private:
  page_id_t page_id;
  size_t size;

  page_id_t bucket_page_ids[0];
};
}  // namespace miniKV

#endif  // MINIKV_HASHTABLEHEADERPAGE_H
