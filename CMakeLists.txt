cmake_minimum_required(VERSION 3.5) # 设置CMake最小版本

project(myServer) # 设置工程名

add_executable(${PROJECT_NAME} main.cpp) # 生成可执行文件

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    pthread
    mylog
)

add_subdirectory(test)
add_subdirectory(log)

