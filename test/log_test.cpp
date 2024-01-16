#include "log.h"

using std::cout;
using std::endl;

int m_iCloseLog = 0;

void syncLogTest() {
    Log *logger = Log::getInstance();

    string szPath = "other/log/testLog";
    // 同步
    bool bRet = logger->init(szPath.c_str(), m_iCloseLog);

    for(int i = 0; i < 10; i++) {
        LOG_INFO("同步log%d", i);
    }

    cout << "测试日志写入成功,日志路径: other/log/" << endl;
}

void asyncLogTest() {
    Log *logger = Log::getInstance();

    string szPath = "other/log/testLog";
    // 异步
    bool bRet = logger->init(szPath.c_str(), m_iCloseLog, 8192, 500000, 20);
    if(!bRet) {
        cout << "初始化失败,请确定文件夹是否存在" << endl;
        exit(-1);
    }

    for(int i = 0; i < 20; i++) {
        LOG_INFO("异步测试%d", i);
    }

    // 异步,不能结束太快,否则写log线程可能还没完成
    for(;;){}
}

int main() {
    syncLogTest();
    // asyncLogTest();

    return 0;
}

