#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>

#include "mysqlPool.h"

mysqlPool::mysqlPool() {
    m_iCurConn = 0;
    m_iFreeConn = 0;
}

mysqlPool *mysqlPool::getInstance() {
    static mysqlPool connPool;
    return &connPool;
}

void mysqlPool::init(string szUrl, string szUser, string szPassWord, string szDBName, int iPort, int iMaxConn, int iCloseLog) {
    m_szUrl = szUrl;
    m_szUser = szUser;
    m_szPassWord = szPassWord;
    m_szDatabaseName = szDBName;
    m_iCloseLog = iCloseLog;

    for(int i = 0; i < iMaxConn; i++) {
        MYSQL *con = NULL;
        con = mysql_init(con);

        if(con == NULL) {
            LOG_ERROR("mysql_init Error");
            exit(1);
        }

        con = mysql_real_connect(con, m_szUrl.c_str(), m_szUser.c_str(), m_szPassWord.c_str(), m_szDatabaseName.c_str(), iPort, NULL, 0);
        if(con == NULL) {
            LOG_ERROR("mysql_real_connect Error");
            exit(1);
        }

        m_connList.push_back(con);
        ++m_iFreeConn;
    }

    m_reserve = sem(m_iFreeConn);
    m_iMaxConn = m_iFreeConn;
}

// 从数据库连接池返回一个可用链接
MYSQL *mysqlPool::GetConnection() {
    MYSQL *con = NULL;

    if(0 == m_connList.size()) {
        return NULL;
    }

    m_reserve.wait();

    m_lock.lock();

    con = m_connList.front();
    m_connList.pop_front();
    --m_iFreeConn;
    ++m_iCurConn;

    m_lock.unlock();

    return con;
}

bool mysqlPool::ReleaseConnection(MYSQL *con) {
    if(NULL == con) {
        return false;
    }

    m_lock.lock();

    m_connList.push_back(con);
    ++m_iFreeConn;
    --m_iCurConn;

    m_lock.unlock();

    m_reserve.post();
    return true;
}

// 销毁数据库连接池
void mysqlPool::DestroyPool() {
    m_lock.lock();

    if(m_connList.size() > 0) {
        for(auto& it : m_connList) {
            MYSQL *con = it;
            mysql_close(con);
        }
        m_iCurConn = 0;
        m_iFreeConn = 0;
        m_connList.clear();
    }

    m_lock.unlock();
}

int mysqlPool::GetFreeConn() {
    return this->m_iFreeConn;
}

mysqlPool::~mysqlPool() {
    DestroyPool();
}

connectionRAII::connectionRAII(MYSQL **con, mysqlPool *connPool) {
    *con = connPool->GetConnection();

    m_conRAII = *con;
    m_poolRAII = connPool;
}

connectionRAII::~connectionRAII() {
    m_poolRAII->ReleaseConnection(m_conRAII);
}


