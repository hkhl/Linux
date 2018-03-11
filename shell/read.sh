#!/bin/bash

echo -n "Fist name:"  #回车不换行 -n
read fistname
echo -e "FIST NAME : $fistname\n"

echo -n "Last name: name:"  #回车不换行 -n
read lastname1 lastname2    #一次定义多个变量, 空格分割, 多余的给最后一个变量 回车结束
echo -e "LAST NAME1 : $lastname1\n"
echo -e "LAST NAME2 : $lastname2\n"   

