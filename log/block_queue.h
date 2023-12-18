/*
 * 阻塞队列,m_back = (m_back + 1) % m_max_size;
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
class block_queue {
public:
    block_queue(int max_size=1000) {
        if(max_size <= 0) {
            exit(-1);
        }
    }
};

#endif
