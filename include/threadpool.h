#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__


#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "locker.h"
#include "mysqlPool.h"

template <typename T>
class threadpool {
public:
    /* *
     *  iThreadNum 是线程池中线程数, iMaxReq是请求队列种的请求数量
     * */
    threadpool(int iActModel, mysqlPool* sqlPool, int iThreadNum = 8, int iMaxReq = 10000);

    ~threadpool();

    bool append(T *request, int state);

    bool appendP(T *request);

private:
    // 工作线程运行的函数,不断从工作队列去除任务执行
    static void *worker(void* arg);
    void run();

private:
    int m_iThreadNum;           // 线程池中线程数
    int m_iMaxRequest;          // 请求队列中最大请求数
    pthread_t *m_threads;       // 描述线程池的数组,大小为m_iThreadNum
    std::list<T *> m_workQueue; // 请求队列
    locker m_queueLocker;       // 保护请求队列的互斥锁
    sem m_queueStat;            // 是否有任务需要处理
    mysqlPool *m_mysqlPool;     // 数据库
    int m_iActorModel;          // 模型切换
};

template<typename T>
threadpool::threadpool(int iActModel, mysqlPool* sqlPool, int iThreadNum = 8, int iMaxReq = 10000) : m_iActorModel(iActModel), m_iThreadNum(iThreadNum), m_iMaxRequest(iMaxReq), m_threads(nullptr), m_mysqlPool(sqlPool) {
    if(iThreadNum <= 0 || iMaxReq <= 0) {
        throw std::exception();
    }

    m_threads = new pthread_t[m_iThreadNum];
    if(m_threads == nullptr) {
        throw std::exception();
    }

    // 初始化线程
    for(int i = 0; i < m_iThreadNum; i++) {
        if(pthread_create(&m_threads[i], NULL, worker, this) != 0) {
            delete[] m_threads;
            throw std::exception();
        }

        if(pthread_detach(m_threads[i])) {
            delete[] m_threads;
            throw std::exception();
        }
    }
}

template<typename T>
thradpool<T>::~threadpool() {
    delete[] m_threads;
}

template<typename T>
bool threadpool<T>::append(T *request, int iState) {
    m_queueLocker.lock();

    if(m_workQueue.size() >= m_iMaxRequest) {
        m_queueLocker.unlock();
        return false;
    }

    request->m_iState = iState;
    m_workQueue.emplace_back(request);
    m_queueLocker.unlock();
    m_queueStat.post();

    return true;
}

template<typename T>
bool threadpool::appendP(T *request) {
    m_queueLocker.lock();

    if(m_workQueue.size() >= m_iMaxRequest) {
        m_queueLocker.unlock();
        return false;
    }

    m_workQueue.emplace_back(request);
    m_queueLocker.unlock();
    m_queueStat.post();

    return true;
}

template<typename T>
void* threadpool<T>::worker(void *arg) {
    threadpool *pool = (thread*)arg;
    pool->run();

    return pool;
}

template<typename T>
void threadpool<T>::run() {
    while(true) {
        m_queueStat.wait();
        m_queueLocker.lock();
        if(m_workQueue.empty()) {
            m_queueLocker.unlock();
            continue;
        }

        T *request = m_workQueue.front();
        m_workQueue.pop_front();
        m_queueLocker.unlock();

        if(!request) {
            contunue;
        }

        if(1 == m_iActorModel) {
            if(0 == request->m_iState) {

            }
        }
    }
}

#endif

