
add_executable(log_test log_test.cpp) # 生成可执行文件

target_include_directories(log_test
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/log
)

target_link_libraries(log_test
    mylog
)

