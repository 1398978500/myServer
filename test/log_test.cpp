#include "log.h"

using std::cout;
using std::endl;

int main() {
    int m_close_log = 0;
    Log *logger = Log::get_instance();

    // 同步
    // bool bRet = logger->init("other/log/testLog", m_close_log);
    // 异步
    bool bRet = logger->init("other/log/testLog", m_close_log, 8192, 500000, 20);
    if(!bRet) {
        cout << "初始化失败,请确定文件夹是否存在" << endl;
        exit(-1);
    }

    for(int i = 0; i < 20; i++) {
        LOG_INFO("异步测试%d", i);
    }

    // 异步,结束太快,写log线程可能还没启动
    for(;;){}

    return 0;
}

