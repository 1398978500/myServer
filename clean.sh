#!/bin/bash


if [[ $1 == "all" ]]
then
    echo "rm ./build -r"
    rm ./build -r
elif [[ $1 == "log" ]]
then
    echo "rm ./other/log -r"
    rm ./other/log/*
else
    echo "./clean all 清理包括cmake的所有缓存 cmake文件有修改时要用此命令"
    echo "cd build && make clean && cd -"
    cd build && make clean && cd -
fi


