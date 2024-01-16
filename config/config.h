#ifndef __CONFIG_H__
#define __CONFIG_H__

struct Config {
    Config();
    ~Config(){}

    void parseArg(int argc, char* argv[]);

    // 端口号
    int m_iPort;

    // 日志写入方式
    int m_iLogWrite;

    // 触发组合模式
    int m_iTrigMode;

    // listenfd 触发模式
    int m_iListenTrigMode;

    // connfd 触发模式
    int m_iConnTrigMode;

    // 优雅关闭连接
    int m_iOptLinger;

    // 数据库连接池数量
    int m_iSqlNum;

    // 线程池内的线程数量
    int m_iThreadNum;

    // 是否关闭日志
    int m_iCloseLog;

    // 并发模型选择
    int m_iActorMode;
};

#endif

