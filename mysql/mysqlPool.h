#ifndef __MYSQL_POOL__
#define __MYSQL_POOL__

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>
#include "locker.h"
// #include "log.h"

using std::list;
using std::string;

class mysqlPool {
public:
    MYSQL *GetConnection(); // 获取数据库连接
    bool ReleaseConnection(MYSQL *con);  // 释放连接
    int GetFreeConn();                   // 获取连接
    void DestroyPool();                  // 销毁所有连接

    // 单例
    static mysqlPool *getInstance();

    // 初始化成功返回0
    int init(string szUrl, string szUser, string szPassword, string szDataBaseName, int iPort, int iMaxConn, int iCloseLog);

private:
    mysqlPool();
    ~mysqlPool();

    int m_iMaxConn;  // 最大连接数
    int m_iCurConn;  // 已使用连接数
    int m_iFreeConn; // 当前空闲连接数
    locker m_lock;
    list<MYSQL *> m_connList; // 连接池
    sem m_reserve;

public:
    string m_szUrl;
    string m_szPort;
    string m_szUser;
    string m_szPassWord;
    string m_szDatabaseName;
    int m_iCloseLog;

};

class connectionRAII{
public:
    connectionRAII(MYSQL **con, mysqlPool *connPool);
    ~connectionRAII();

private:
    MYSQL *m_conRAII;
    mysqlPool *m_poolRAII;
};


#endif

