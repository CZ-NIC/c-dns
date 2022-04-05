find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(tests tests.cpp)
target_link_libraries(tests Threads::Threads ZLIB::ZLIB ${LIBLZMA_LIBRARIES} cdns gtest gtest_main)
target_include_directories(tests PUBLIC ${LIBLZMA_INCLUDE_DIRS})
