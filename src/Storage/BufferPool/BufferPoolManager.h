//
// Created by 何智强 on 2021/10/2.
//

#ifndef MINIKV_BUFFERPOOLMANAGER_H
#define MINIKV_BUFFERPOOLMANAGER_H

#include "src/Common/Config.h"
#include "src/Storage/Disk/DiskManager.h"
#include "src/Storage/Page/Page.h"
#include "src/Storage/BufferPool/IReplacer.h"
#include <memory>
#include <list>
#include <unordered_map>

namespace miniKV {
class BufferPoolManager{
public:
    BufferPoolManager() = default;
    BufferPoolManager(size_t pool_size_, std::shared_ptr<DiskManager> disk_manager_);

    std::shared_ptr<Page> FetchPage(miniKV::page_id_t page_id);
    bool UnpinPage(page_id_t page_id, bool is_dirty);
    bool FlushPage(page_id_t page_id);
    std::shared_ptr<Page> NewPage();
    bool DeletePage(page_id_t page_id);
    void FlushAllPages();

private:
    std::size_t pool_size;
    std::shared_ptr<DiskManager> disk_manager;
    std::unique_ptr<IReplacer> replacer;
    std::vector<std::shared_ptr<Page>> pages;
    std::list<frame_id_t> free_list;
    std::mutex latch;
    std::unordered_map<page_id_t, frame_id_t> page_table;
};
}



#endif //MINIKV_BUFFERPOOLMANAGER_H
