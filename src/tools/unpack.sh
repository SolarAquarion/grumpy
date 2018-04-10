#!/bin/sh
pwd
if [ ! -f "../src/sqlite/sqlite3.h" ];then
    cd "../src/sqlite" || exit 1
    unzip sqlite-amalgamation-3220000.zip || exit 1
    mv sqlite-amalgamation-3220000/* . || exit 1
    rm -rf sqlite-amalgamation-3220000 || exit 1
    cd - || exit 1
fi
