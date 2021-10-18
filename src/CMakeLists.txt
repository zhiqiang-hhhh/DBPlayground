file(GLOB_RECURSE miniKV_sources ${PROJECT_SOURCE_DIR}/src/*/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*/*[^HASH].cpp Core/MiniKV.cpp)

list(REMOVE_ITEM miniKV_sources ${PROJECT_SOURCE_DIR}/src/Container/LeanerProbeHashTable.h ${PROJECT_SOURCE_DIR}/src/Container/LeanerProbeHashTable.cpp)
list(REMOVE_ITEM miniKV_sources ${PROJECT_SOURCE_DIR}/src/Storage/Page/HashTablePage.h ${PROJECT_SOURCE_DIR}/src/Storage/Page/HashTablePage.cpp)
list(REMOVE_ITEM miniKV_sources ${PROJECT_SOURCE_DIR}/src/Storage/Page/HashTableHeaderPage.h ${PROJECT_SOURCE_DIR}/src/Storage/Page/HashTableHeaderPage.cpp)

add_library(miniKV_lib STATIC ${miniKV_sources})
target_link_libraries(miniKV_lib glog::glog)

