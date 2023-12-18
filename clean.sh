#!/bin/bash


if [[ $1 == "all" ]]
then
    echo "rm ./build -r"
    rm ./build -r
else
    echo "./clean all 清理包括cmake的所有缓存 cmake文件有修改时要用此命令"
    echo "cd build && make clean && cd -"
    cd build && make clean && cd -
fi


