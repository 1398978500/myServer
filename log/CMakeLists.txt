
add_library(mylog SHARED
    log.cpp
)

target_include_directories(mylog
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(mylog
    pthread
)

