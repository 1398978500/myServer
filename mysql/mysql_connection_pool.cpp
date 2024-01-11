#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>

#include "mysql_connection_pool.h"

connection_pool::connection_pool() {
    m_iCurConn = 0;
    m_iFreeConn = 0;
}

connection_pool *connection_pool::GetInstance() {
    static connection_pool connPool;
    return &connPool;
}

void connection_pool::init(string szUrl, string szUser, string szPassWord, string szDBName, int iPort, int iMaxConn, int iCloseLog) {
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
MYSQL *connection_pool::GetConnection() {
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

bool connection_pool::ReleaseConnection(MYSQL *con) {
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
void connection_pool::DestroyPool() {
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

int connection_pool::GetFreeConn() {
    return this->m_iFreeConn;
}

connection_pool::~connection_pool() {
    DestroyPool();
}

connectionRAII::connectionRAII(MYSQL **con, connection_pool *connPool) {
    *con = connPool->GetConnection();

    m_conRAII = *con;
    m_poolRAII = connPool;
}

connectionRAII::~connectionRAII() {
    m_poolRAII->ReleaseConnection(m_conRAII);
}


