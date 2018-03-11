#!/bin/bash

echo "ctrl+ d quit"
while echo  -n "please input your name:"; read name
do 
    echo "Yo, you name is $name"
done


#
echo -e "\n读取文件 文件名写在done后"
while read var
do 
    echo "$var"
done <name.txt
