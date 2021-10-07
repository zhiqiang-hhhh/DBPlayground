//
// Created by 何智强 on 2021/10/2.
//

#include "src/Storage/BufferPool/BufferPoolManager.h"
#include "src/Storage/BufferPool/LRUReplaceer.h"

namespace miniKV {

    BufferPoolManager::BufferPoolManager(size_t pool_size_, std::shared_ptr<DiskManager> disk_manager_)
    : pool_size(pool_size_), disk_manager(disk_manager_)
    {
        for (int i = 0; i < pool_size; ++i) {
            pages.push_back(std::shared_ptr<Page>(new Page));
            free_list.push_back(i);
        }

        replacer = std::make_unique<LRUReplacer>(pool_size);
    }

    std::shared_ptr<Page> BufferPoolManager::FetchPage(page_id_t page_id) {
        // 1.     Search the page table for the requested page (P).
        // 1.1    If P exists, pin it and return it immediately.
        // 1.2    If P does not exist, find a replacement page (R) from either the
        // free list or the replacer.
        //        Note that pages are always found from the free list first.
        // 2.     If R is dirty, write it back to the disk.
        // 3.     Delete R from the page table and insert P.
        // 4.     Update P's metadata, read in the page content from disk, and then
        // return a pointer to P.
        std::lock_guard<std::mutex> guard{latch};

        if (page_table.count(page_id) != 0) {
            frame_id_t frame_id = page_table[page_id];
            auto page_ptr = pages.at(frame_id);
            ++page_ptr->pin_count;
            replacer->Pin(frame_id);
            return page_ptr;
        }

        if (free_list.empty()) {
            frame_id_t victimFrameId;
            if (replacer->Victim(&victimFrameId)) {
                auto page_ptr = pages.at(victimFrameId);
                if (page_ptr->IsDirty()) {
                    disk_manager->WritePage(page_ptr->GetPageId(), page_ptr->GetData());
                }
                page_table.erase(page_ptr->page_id);
                page_ptr->ResetMemory();
                page_ptr->page_id = INVALID_PAGE_ID;
                page_ptr->is_dirty = false;
                free_list.emplace_back(victimFrameId);
            } else {
                // Can not find any victim frame in replacer.
                return nullptr;
            }
        }

        frame_id_t freeFrameID = free_list.back();
        free_list.pop_back();
        auto page_ptr = pages.at(freeFrameID);
        ++page_ptr->pin_count;
        page_ptr->page_id = page_id;
        page_ptr->is_dirty = false;
        replacer->Pin(freeFrameID);
        page_table[page_id] = freeFrameID;
        try {
            disk_manager->ReadPage(page_id, page_ptr->GetData());
        } catch (...) {
            ;
        }
        return page_ptr;
    }  // namespace bustub

    bool BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty) {
        std::lock_guard<std::mutex> guard{latch};
        if (page_table.count(page_id) == 0) {
            return false;
        }

        frame_id_t frame_id = page_table[page_id];
        auto page_ptr = pages.at(frame_id);
        // If page is not dirty, set its state to new state
        // Or just skip new state
        if (!page_ptr->is_dirty) {
            page_ptr->is_dirty = is_dirty;
        }

        if (page_ptr->pin_count <= 0) {
            return false;
        }

        if (--page_ptr->pin_count == 0) {
            replacer->Unpin(frame_id);
        }
        return true;
    }

    bool BufferPoolManager::FlushPage(page_id_t page_id) {
        std::lock_guard<std::mutex> guard{latch};
        // If page is not in buffer
        if (page_table.count(page_id) == 0) {
            return false;
        }

        frame_id_t frame_id = page_table[page_id];
        auto page_ptr = pages.at(frame_id);
        disk_manager->WritePage(page_id, page_ptr->GetData());
        page_ptr->is_dirty = false;
        return true;
    }

    std::shared_ptr<Page> BufferPoolManager::NewPage() {
        // 0.   Make sure you call DiskManager::AllocatePage!
        // 1.   If all the pages in the buffer pool are pinned, return nullptr.
        // 2.   Pick a victim page P from either the free list or the replacer. Always
        // pick from the free list first.
        // 3.   Update P's metadata, zero out memory and add P to the page table.
        // 4.   Set the page ID output parameter. Return a pointer to P.
        std::lock_guard<std::mutex> guard{latch};

        /*
         * Free list is empty.
         * Find a victim page from replacer, flush it to disk
         */
        if (free_list.empty()) {
            if (replacer->Size() == 0) {
                // There is no unpinned pages in replacer.
                return nullptr;
            }
            frame_id_t victim_frame;
            if (replacer->Victim(&victim_frame)) {
                auto page_ptr = pages.at(victim_frame);
                if (page_ptr->IsDirty()) {
                    disk_manager->WritePage(page_ptr->GetPageId(), page_ptr->GetData());
                }
                page_table.erase(page_ptr->GetPageId());
                page_ptr->ResetMemory();
                page_ptr->page_id = INVALID_PAGE_ID;
                page_ptr->is_dirty = false;
                free_list.push_back(victim_frame);
            } else {
                // Can not find any victim frame in replacer.
                return nullptr;
            }
        }

        // At least one free frame in free_list here
        frame_id_t free_frame = free_list.front();
        auto freePage = pages.at(free_frame);
        page_id_t newPageID = disk_manager->AllocatePage();
        freePage->page_id = newPageID;
        page_table[newPageID] = free_frame;
        replacer->Pin(free_frame);
        freePage->pin_count = 1;
        freePage->is_dirty = false;
        free_list.pop_front();
        std::cout << "Created a new page, page_id: " << freePage->page_id << std::endl;
        return freePage;
    }  // namespace bustub

    bool BufferPoolManager::DeletePage(page_id_t page_id) {
        // 0.   Make sure you call DiskManager::DeallocatePage!
        // 1.   Search the page table for the requested page (P).
        // 1.   If P does not exist, return true.
        // 2.   If P exists, but has a non-zero pin-count, return false. Someone is
        // using the page.
        // 3.   Otherwise, P can be deleted. Remove P from the page table, reset its
        // metadata and return it to the free list.
        std::lock_guard<std::mutex> guard{latch};

        if (page_table.count(page_id) == 0) {
            return true;
        }

        frame_id_t frameId = page_table[page_id];
        auto page_ptr = pages.at(frameId);
        if (page_ptr->GetPinCount() != 0) {
        }

        disk_manager->DeallocatePage(page_id);
        page_ptr->ResetMemory();
        page_ptr->is_dirty = false;
        page_ptr->page_id = INVALID_PAGE_ID;
        page_table.erase(page_id);
        replacer->Unpin(frameId);
        free_list.push_back(frameId);
        return true;
    }

    void BufferPoolManager::FlushAllPages() {
        std::lock_guard<std::mutex> guard{latch};
        for (auto item : page_table) {
            page_id_t page_id = item.first;
            frame_id_t frame_id = item.second;
            auto page = pages.at(frame_id);
            disk_manager->WritePage(page_id, page->GetData());
            page->is_dirty = false;
        }
    }


}