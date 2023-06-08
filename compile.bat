@echo off

set LIBRARY_PATH=.

g++ -Wall -std=c++20 -o app.exe main.cpp SQLite3.cpp -lsqlite3

set LD_LIBRARY_PATH=.

app.exe