#include "log.h"

using std::cout;
using std::endl;

int main() {
    int m_close_log = 0;
    Log *logger = Log::get_instance();

    bool bRet = logger->init("other/log/testLog", m_close_log);
    if(!bRet) {
        cout << "初始化失败,请确定文件夹是否存在" << endl;
        exit(-1);
    }

    LOG_INFO("测试");

    return 0;
}

