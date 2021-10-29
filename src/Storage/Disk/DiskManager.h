//
// Created by 何智强 on 2021/10/3.
//

#ifndef MINIKV_DISKMANAGER_H
#define MINIKV_DISKMANAGER_H

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include "src/Common/Config.h"

namespace miniKV {

class DiskManager {
 public:
  DiskManager() = delete;
  DiskManager(std::string db_file_) : db_file_name(db_file_), next_page_id(0) {
    std::string::size_type n = db_file_name.rfind('.');
    if (n == std::string::npos) {
      std::cout << "wrong file format";
      return;
    }

    db_io.open(db_file_name, std::ios::binary | std::ios::in | std::ios::out);
    if (!db_io.is_open()) {
      db_io.clear();
      // create a new file
      db_io.open(db_file_name, std::ios::binary | std::ios::trunc | std::ios::out);
      db_io.close();
      // reopen with original mode
      db_io.open(db_file_name, std::ios::binary | std::ios::in | std::ios::out);
      if (!db_io.is_open()) {
        throw std::runtime_error("can't open db file");
      }
    }
  }

  void ReadPage(page_id_t page_id, char *page_data);
  void WritePage(page_id_t page_id, char *page_data);

  page_id_t AllocatePage();
  void DeallocatePage(page_id_t page_id);

 private:
  const std::string db_file_name;
  std::fstream db_io;
  std::atomic<page_id_t> next_page_id;

  int GetFileSize() const;
};
}  // namespace miniKV

#endif  // MINIKV_DISKMANAGER_H
