//
// Created by 何智强 on 2021/10/2.
//

#ifndef MINIKV_BUFFERPOOLMANAGER_H
#define MINIKV_BUFFERPOOLMANAGER_H

#include <list>
#include <unordered_map>

#include "Common/Config.h"
#include "Storage/BufferPool/IReplacer.h"
#include "Storage/Disk/DiskManager.h"
#include "Storage/Page/Page.h"

namespace miniKV {
class BufferPoolManager {
 public:
  BufferPoolManager() = default;
  /**
   * Create a new buffer pool manager.
   * @param slot_num Number of pages in buffer, page size is defiend in src/Common/Config.h.
   * @param disk_manager_ A buffer pool manager.
   */
  BufferPoolManager(size_t slot_num, std::shared_ptr<DiskManager> disk_manager_);

  std::shared_ptr<Page> FetchPage(miniKV::page_id_t page_id);
  bool UnpinPage(page_id_t page_id, bool is_dirty);
  bool FlushPage(page_id_t page_id);
  std::shared_ptr<Page> NewPage();
  bool DeletePage(page_id_t page_id);
  void FlushAllPages();

 private:
  std::size_t slot_num;
  std::shared_ptr<DiskManager> disk_manager;
  std::unique_ptr<IReplacer> replacer;
  std::vector<std::shared_ptr<Page>> pages;
  std::list<frame_id_t> free_list;
  std::mutex latch;
  std::unordered_map<page_id_t, frame_id_t> page_table;
};
}  // namespace miniKV

#endif  // MINIKV_BUFFERPOOLMANAGER_H
