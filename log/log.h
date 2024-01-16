#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <pthread.h>
#include "blockQueue.h"

using std::string;

class Log {
public:
    // c++11后,使用局部变量懒汉不用加锁
    static Log *getInstance() {
        static Log instance;
        return &instance;
    }

    static void *flushLogThread(void *args) {
        Log::getInstance()->asyncWriteLog();
        return nullptr;
    }

    // 可选择的参数有日志文件,日志缓冲区大小,最大行数和最长日志条队列
    bool init(const char *file_name, int close_log, int log_buf_size = 8192, int split_lines = 500000, int max_queue_size = 0);

    //void writeLog(int level, const char *format, ...);
    void writeLog(const char *file, const char *function, int line, int level, const char *format, ...);

    void flush(void);

private:
    Log();
    virtual ~Log();
    void *asyncWriteLog() {
        std::cout << "写log线程启动" << std::endl;
        string single_log;
        // 从阻塞队列取出一个日志string, 写入文件
        while(m_logQueue->pop(single_log)) {
            m_mutex.lock();
            fputs(single_log.c_str(), m_fp);
            // 这里最好刷新一下,否则可能日志会丢失
            fflush(m_fp);
            m_mutex.unlock();
        }

        return nullptr;
    }


private:
    char m_dirName[128]; // 路径名
    char m_logName[128]; // log文件名
    int m_iSplitLines;  // 日志最大行数
    int m_iLogBufSize; // 日志缓冲区大小
    long long m_llCount;  // 日志行数记录
    int m_iToday;        // 按天分类,记录当前是哪一天
    FILE *m_fp;         // log文件指针
    char *m_buf;
    blockQueue<string> *m_logQueue; // 阻塞队列
    bool m_bIsAsync;                  // 是否同步标志位
    locker m_mutex;
    int m_iCloseLog; // 关闭日志
};

#define LOG_DEBUG(format, ...) if(0 == m_iCloseLog) { Log::getInstance()->writeLog(__FILE__, __FUNCTION__, __LINE__, 0, format, ##__VA_ARGS__); Log::getInstance()->flush();}
#define LOG_INFO(format, ...) if(0 == m_iCloseLog) { Log::getInstance()->writeLog(__FILE__, __FUNCTION__, __LINE__, 1, format, ##__VA_ARGS__); Log::getInstance()->flush();}
#define LOG_WARN(format, ...) if(0 == m_iCloseLog) { Log::getInstance()->writeLog(__FILE__, __FUNCTION__, __LINE__, 2, format, ##__VA_ARGS__); Log::getInstance()->flush();}
#define LOG_ERROR(format, ...) if(0 == m_iCloseLog) { Log::getInstance()->writeLog(__FILE__, __FUNCTION__, __LINE__, 3, format, ##__VA_ARGS__); Log::getInstance()->flush();}


#endif

