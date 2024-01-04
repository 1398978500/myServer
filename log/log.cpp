#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

#include "log.h"

#define LOG_FULL_NAME_LENGTH (512)

Log::Log() {
    m_llCount = 0;
    m_bIsAsync = false;
}

Log::~Log() {
    if(m_fp != NULL) {
        fclose(m_fp);
    }
}

// 异步需要设置阻塞队列长度,同步不需要
bool Log::init(const char *file_name, int close_log, int log_buf_size, int split_lines, int max_queue_size) {
    // 如果设置了max_queue_size, 设置为异步
    if(max_queue_size >= 1) {
        m_bIsAsync = true;
        m_logQueue = new block_queue<string>(max_queue_size);
        pthread_t tid;
        // flushLogThread是回调函数船舰线程异步写日志
        pthread_create(&tid, NULL, flushLogThread, NULL);
    }

    m_iCloseLog = close_log;
    m_iLogBufSize = log_buf_size;
    m_buf = new char[m_iLogBufSize];
    memset(m_buf, 0, m_iLogBufSize);
    m_iSplitLines = split_lines;

    time_t t = time(NULL);
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;

    const char *p = strrchr(file_name, '/');
    char log_full_name[LOG_FULL_NAME_LENGTH] = {0};

    if(p == NULL) {
        snprintf(log_full_name, LOG_FULL_NAME_LENGTH - 1, "%d_%02d_%02d_%s", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, file_name);
    } else {
        strcpy(m_logName, p + 1);
        strncpy(m_dirName, file_name, p - file_name + 1);
        snprintf(log_full_name, LOG_FULL_NAME_LENGTH - 1, "%s%d_%02d_%02d_%s", m_dirName, my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, m_logName);
    }

    m_iToday = my_tm.tm_mday;

    m_fp = fopen(log_full_name, "a");
    if(m_fp == NULL) {
        return false;
    }

    return true;
}

void Log::writeLog(const char *file, const char *function, int line, int level, const char *format, ...) {
    struct timeval now = {0, 0};
    gettimeofday(&now, NULL);
    time_t t = now.tv_sec;
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;
    char s[16] = {0};
    switch (level) {
        case 0:
            strcpy(s, "[debug]:");
            break;
        case 1:
            strcpy(s, "[info]:");
            break;
        case 2:
            strcpy(s, "[warn]:");
            break;
        case 3:
            strcpy(s, "[error]:");
            break;
        default:
            strcpy(s, "[info]:");
            break;
    }

    // 写入一个log, 对m_llCount++, m_iSplitLines最大行数
    m_mutex.lock();

    m_llCount++;

    // 日志切换
    if(m_iToday != my_tm.tm_mday || m_llCount % m_iSplitLines == 0) {
        char new_log[LOG_FULL_NAME_LENGTH] = {0};
        fflush(m_fp);
        fclose(m_fp);

        char tail[16] = {0};
        snprintf(tail, 16, "%d_%02d_%02d_", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday);

        if(m_iToday != my_tm.tm_mday) {
            snprintf(new_log, LOG_FULL_NAME_LENGTH - 1, "%s%s%s", m_dirName, tail, m_logName);
            m_iToday = my_tm.tm_mday;
            m_llCount = 0;
        } else {
            snprintf(new_log, LOG_FULL_NAME_LENGTH - 1, "%s%s%s.%lld", m_dirName, tail, m_logName, m_llCount / m_iSplitLines);
        }
        m_fp = fopen(new_log, "a");
    }

    m_mutex.unlock();

    va_list valst;
    va_start(valst, format);

    string log_str;
    m_mutex.lock();

    // 写入得具体时间内容格式
    int n = snprintf(m_buf, 1024, "[%s] [%s:line:%d] %d-%02d-%02d %02d:%02d:%02d/%06ld %s", 
            file, function, line,
            my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday,
            my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec, now.tv_usec, s);

    int m = vsnprintf(m_buf + n, m_iLogBufSize - n - 1, format, valst);
    m_buf[m + n] = '\n';
    m_buf[m + n + 1] = '\0';
    log_str = m_buf;


    m_mutex.unlock();

    if(m_bIsAsync && !m_logQueue->full()) {
        m_logQueue->push(log_str);
    } else {
        m_mutex.lock();
        fputs(log_str.c_str(), m_fp);
        m_mutex.unlock();
    }

    va_end(valst);
}

void Log::flush() {
    m_mutex.lock();
    fflush(m_fp);
    m_mutex.unlock();
}

