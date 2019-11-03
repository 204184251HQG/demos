#!/bin/bash

cd globalfifo
make
cd -

gcc -g -o globalfifo_epoll globalfifo_epoll.c