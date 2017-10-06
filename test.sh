#!/bin/sh
g++ -std=c++14 -stdlib=libc++ -lssl -lcrypto -Wall -lboost_system main.cpp
./a.out