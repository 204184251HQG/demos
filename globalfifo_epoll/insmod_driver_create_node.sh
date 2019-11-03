#!/bin/bash

if [ "${1}" == "-u" ];then
    sudo rmmod globalfifo/globalfifo.ko
    sudo rm -rf /dev/globalfifo
    exit 0
fi

sudo insmod globalfifo/globalfifo.ko

sudo mknod /dev/globalfifo -m 666 c 231 0