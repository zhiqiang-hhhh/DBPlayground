//
// Created by 何智强 on 2021/10/3.
//

#include "DiskManager.h"

#include <sys/stat.h>

namespace miniKV {

page_id_t DiskManager::AllocatePage() { return next_page_id++; }

void DiskManager::ReadPage(page_id_t page_id, char *page_data) {
  int off_set = page_id * PAGE_SIZE;
  if (off_set > GetFileSize()) {
    throw std::runtime_error("page id out of range");
  }

  db_io.seekp(page_id * PAGE_SIZE);
  db_io.read(page_data, PAGE_SIZE);

  int read_count = db_io.gcount();
  if (read_count < PAGE_SIZE) {
    db_io.clear();
    memset(page_data + read_count, 0, PAGE_SIZE - read_count);
  }
}

void DiskManager::WritePage(page_id_t page_id, char *page_data) {
  size_t offset = static_cast<size_t>(page_id) * PAGE_SIZE;
  db_io.seekp(offset);
  db_io.write(page_data, PAGE_SIZE);

  /// Flush to disk SYNC
  db_io.flush();
}

int DiskManager::GetFileSize() const {
  struct stat stat_buf;
  int rc = stat(db_file_name.c_str(), &stat_buf);
  return rc == 0 ? static_cast<int>(stat_buf.st_size) : -1;
}

void DiskManager::DeallocatePage(__attribute__((unused)) page_id_t page_id) {}

}  // namespace miniKV