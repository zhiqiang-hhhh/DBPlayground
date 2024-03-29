#include <iostream>
#include <vector>

#include "Core/MiniKV.h"


int main(int argc, char** argv) {
   miniKV::MiniKV db;

   std::vector<std::pair<miniKV::key_t , miniKV::value_t>> entries;
   for (int i = 0; i < 100; ++i) {
       entries.push_back(std::pair<miniKV::key_t, miniKV::value_t>(i, i + 100));
   }

   for (auto entry : entries) {
       db.insert(entry.first, entry.second);
   }

   for (auto entry : entries) {
       std::cout << entry.first << " : " << entry.second
       <<  " in database: " << entry.first << " : " <<db.get(entry.first) << std::endl;
   } 
   
   return 0;
}
