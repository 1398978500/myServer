#!/bin/bash

# 编译脚本

flag=""

for arg in $*;
do
    if [[ ${arg} =~ "verbose" ]]
    then
        flag+=" VERBOSE=1"
    fi
done

[ -d "build" ] || mkdir build

cd build && cmake .. && make -j4 ${flag}

if [[ ${flag} == "" ]]
then
    echo "./compile [argv]
        argv可取:
            verbose:打印详细信息
    "
fi


