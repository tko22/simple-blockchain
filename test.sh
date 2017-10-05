#!/bin/sh
g++ -std=c++14 -stdlib=libc++ -lssl -lcrypto -Wall main.cpp
./a.out