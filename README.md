# myServer
练习编写一个Linux下的c++web服务器,实践网络编程

## 编译
```
./compile.sh

# 查看详细信息
./compile.sh verbose
```

## 清理编译信息
```
./clean.sh

# 清理包含cmake的信息
./clean.sh all
```

## 准备工作
```mysql
# 创建数据库
CREATE DATABASES db_test;

use db_test;

# 创建表格
CREATE TABLE users (
    username VARCHAR(50) PRIMARY KEy,
    password VARCHAR(50) NOT NULL,
);

# 插入测试数据
INSERT INTO users (username, password) VALUES('11', '11'), ('22', '22'), ...;

```
```C
// 编译完成后测试数据库
./build/test/mysql_test
```

## config 参数
```
# "p:l:m:o:s:t:c:a"
./build/test/config_test [-p 8888] [-l 0] ...
```
-p 端口 默认9006
-l 日志写入模式 默认0同步 1异步
-m 触发模式 默认0(LT + LT) 1(LT + ET) 2(ET + LT) 3(ET + ET)";
-o 优雅关闭 默认0关闭 1开启
-s 数据库连接池数量 默认是8
-t 线程池内线程数 默认是8
-c 关闭日志 默认0不关闭 1关闭
-a 并发模型 默认0proactor 1reactor




