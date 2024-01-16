#include <iostream>
#include "mysqlPool.h"

using namespace std;

int main() {
    MYSQL *mysql = NULL;
    MysqlPool *pool = MysqlPool::getInstance();
    if(pool == NULL) {
        cout << "getInstance Error " << endl;
    }
    pool->init("localhost", "test", "test", "dbtest", 3306, 5, 0);
    cout << "init" << endl;

    connectionRAII mysqlcon(&mysql, pool);

    if(mysql_query(mysql, "select username, passwd from user"))
    {
        cout << "mysql_query error" << endl;
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(mysql);
    if(result == NULL) {
        cout << "mysql_store_result error" << endl;
        return -1;
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);

    while(MYSQL_ROW row = mysql_fetch_row(result)) {
        cout << "username : " << row[0] << "password : " << row[1] << endl;
    }


    return 0;
}

