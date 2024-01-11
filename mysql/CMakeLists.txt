

add_library(mysql_connect_pool SHARED
    mysql_connection_pool.cpp

)

target_include_directories(mysql_connect_pool
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/log
)

target_link_libraries(mysql_connect_pool
    pthread
    mysqlclient
    mylog
)

