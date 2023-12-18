#!/bin/bash

# 编译脚本

[ -d "build" ] || mkdir build

cd build && cmake .. && make -j4

