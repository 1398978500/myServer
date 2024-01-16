#include "config.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    Config config;
    config.parseArg(argc, argv);

    cout << "端口:" << config.m_iPort << endl;
    cout << "日志写入模式:" << config.m_iLogWrite << endl;
    cout << "触发模式:" << config.m_iTrigMode << endl;
    cout << "listen模式:" << config.m_iListenTrigMode << endl;
    cout << "conn模式:" << config.m_iConnTrigMode << endl;
    cout << "优雅关闭:" << config.m_iOptLinger << endl;
    cout << "sql连接数量:" << config.m_iSqlNum << endl;
    cout << "线程数:" << config.m_iThreadNum << endl;
    cout << "是否关闭日志:" << config.m_iCloseLog << endl;
    cout << "并发模型:" << config.m_iActorMode << endl;

    return 0;
}

