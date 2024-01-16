#include <unistd.h>
#include <stdlib.h>

#include "config.h"

Config::Config() {
    // 默认参数
    m_iPort = 9006;

    // 日志默认同步
    m_iLogWrite = 0;

    // 默认组合模式: listernfd LT + connfd LT
    m_iTrigMode = 0;

    // 默认listenfd触发模式: LT
    m_iListenTrigMode = 0;

    // 默认connfd触发模式:LT
    m_iConnTrigMode = 0;

    // 优雅关闭连接 默认关闭
    m_iOptLinger = 0;

    // 数据库连接池数量 默认8
    m_iSqlNum = 8;

    // 线程池内线程数量 默认8
    m_iThreadNum = 8;

    // 默认日志不关闭
    m_iCloseLog = 0;

    // 并发模型 默认proactor
    m_iActorMode = 0;
}

void Config::parseArg(int argc, char *argv[]) {
    int iOpt = 0;
    const char *str = "p:l:m:o:s:t:c:a:";
    while((iOpt = getopt(argc, argv, str)) != -1) {
        switch (iOpt) {
            case 'p' : {
                m_iPort = atoi(optarg);
            }
            break;

            case 'l' : {
                m_iLogWrite = atoi(optarg);
            }
            break;

            case 'm' : {
                m_iTrigMode = atoi(optarg);
            }
            break;

            case 'o' : {
                m_iOptLinger = atoi(optarg);
            }
            break;

            case 's' : {
                m_iSqlNum = atoi(optarg);
            }
            break;

            case 't' : {
                m_iThreadNum = atoi(optarg);
            }
            break;

            case 'c' : {
                m_iCloseLog = atoi(optarg);
            }
            break;

            case 'a' : {
                m_iActorMode = atoi(optarg);
            }
            break;

            default :
            break;
        }

    }

}

