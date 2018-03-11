#!/bin/bash
#exec必须和文件描述副一块使用

exec 3<&0 0<name.txt
read read1
read read2

exec 0<&3
echo "name1: $read1"
echo "name2: $read2"
