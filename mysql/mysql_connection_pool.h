#ifndef __MYSQL_CONNECTION_POOL__
#define __MYSQL_CONNECTION_POOL__

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>
#include "locker.h"
#include "log.h"

using std::list;

class connection_pool {
public:
    MYSQL *GetConnection(); // 获取数据库连接
    bool ReleaseConnection(MYSQL *con);  // 释放连接
    int GetFreeConn();                   // 获取连接
    void DestroyPool();                  // 销毁所有连接

    // 单例
    static connection_pool *GetInstance();

    void init(string szUrl, string szUser, string szPassword, string szDataBaseName, int iPort, int iMaxConn, int iCloseLog);

private:
    connection_pool();
    ~connection_pool();

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
    connectionRAII(MYSQL **con, connection_pool *connPool);
    ~connectionRAII();

private:
    MYSQL *m_conRAII;
    connection_pool *m_poolRAII;
};


#endif

