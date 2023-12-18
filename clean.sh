#!/bin/bash

if [[ $1 == "all" ]]
then
    echo "rm ./build -r"
    rm ./build -r
else
    echo "cd build && make clean && cd -"
    cd build && make clean && cd -
fi


