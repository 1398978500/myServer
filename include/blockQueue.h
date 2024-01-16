/*
 * 阻塞队列,m_back = (m_back + 1) % m_iMaxSize;
 * 安全,每个操作都会加锁,操作完成解锁
 */

#ifndef __BLOCK_QUEUE_H__
#define __BLOCK_QUEUE_H__

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "locker.h"

template <class T>
class blockQueue {
public:
    blockQueue(int iMaxSize=1000) {
        if(iMaxSize <= 0) {
            exit(-1);
        }

        m_iMaxSize = iMaxSize;
        m_array = new T[iMaxSize];
        m_size = 0;
        m_front = -1;
        m_back = -1;
    }

    void clear() {
        m_mutex.lock();
        m_size = 0;
        m_front = -1;
        m_back = -1;
        m_mutex.unlock();
    }

    ~blockQueue() {
        m_mutex.lock();
        if(m_array != NULL) {
            delete [] m_array;
        }
        m_mutex.unlock();
    }

    bool full() {
        m_mutex.lock();
        bool bRet = (m_size >= m_iMaxSize);
        m_mutex.unlock();

        return bRet;
    }

    bool empty() {
        m_mutex.lock();
        bool bRet = (0 == m_size);
        m_mutex.unlock();

        return bRet;
    }

    // 返回队首元素
    bool front(T &value) {
        bool bRet = true;

        m_mutex.lock();
        if(0 == m_size) {
            bRet = false;
        } else {
            value = m_array[m_front];
        }
        m_mutex.unlock();

        return bRet;
    }

    // 返回队尾元素
    bool back(T &value) {
        bool bRet = true;

        m_mutex.lock();
        if(0 == m_size) {
            bRet = false;
        } else {
            value = m_array[m_back];
        }
        m_mutex.unlock();

        return bRet;
    }

    int size() {
        int tmp = 0;

        m_mutex.lock();
        tmp = m_size;
        m_mutex.unlock();

        return tmp;
    }

    int iMaxSize() {
        int tmp = 0;

        m_mutex.lock();
        tmp = m_iMaxSize;
        m_mutex.unlock();

        return tmp;
    }

    // 添加元素 需要先唤醒使用队列的线程
    // 当前没有线程等待条件变量,唤醒无意义
    bool push(const T &item) {
        m_mutex.lock();
        if(m_size >= m_iMaxSize) {
            m_cond.broadcast();
            m_mutex.unlock();
            return false;
        }

        m_back = (m_back+1) % m_iMaxSize;
        m_array[m_back] = item;
        m_size++;
        m_cond.broadcast();

        m_mutex.unlock();

        return true;
    }

    // 如果队列没有元素,会等待条件变量
    bool pop(T &item) {
        m_mutex.lock();
        while(m_size <= 0) {
            if(!m_cond.wait(m_mutex.get())) {
                m_mutex.unlock();
                return false;
            }
        }

        m_front = (m_front + 1) % m_iMaxSize;
        item = m_array[m_front];
        m_size--;
        m_mutex.unlock();

        return true;
    }

    // 增加超时处理
    bool pop(T &item, int ms_timeout) {
        struct timespec t = {0, 0};
        struct timeval now = {0,0};

        gettimeofday(&now, NULL);

        m_mutex.lock();
        if(m_size <= 0) {
            t.tv_sec = now.tv_sec + ms_timeout / 1000;
            t.tv_nsec = (ms_timeout % 1000) * 1000;
            if(!m_cond.timewait(m_mutex.get(), t)) {
                m_mutex.unlock();
                return false;
            }
        }

        if(m_size <= 0) {
            m_mutex.unlock();
            return false;
        }

        m_front = (m_front + 1) % m_iMaxSize;
        item = m_array[m_front];
        m_size--;
        m_mutex.unlock();

        return true;
    }

private:
    locker m_mutex;
    cond m_cond;

    T* m_array;
    int m_size;
    int m_iMaxSize;
    int m_front;
    int m_back;
};

#endif


