#!/bin/bash

echo "请输入1-3"
read num
case $num in

1) echo "select 1"
    ;; 
2) echo "select 2"
    ;;
3) echo "select 3"
    ;;
y|Y) echo "select y"
    ;;
*) echo "`base name $0`: this is not between 1 and 3">&2 
    exit;
    ;;
esac
