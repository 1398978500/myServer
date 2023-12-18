cmake_minimum_required(VERSION 3.5) # 设置CMake最小版本

add_executable(locker_test locker_test.cpp) # 生成可执行文件

target_include_directories(locker_test
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

