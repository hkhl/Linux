#!/bin/bash

echo "1:"
test -w dong
#注意if后面加空格
#注意使用中括号
#主语中括号前后有空格


#第一个if版本
if [ $?-eq"0" ] ; then
    echo "success"
else
    echo "error"
fi



#第二个if版本
echo "2:"
[ -w dong ]
if [ $? -eq "0" ] 
then
    echo "success"
else
    echo "error"
fi

#第三个if版本

if [ "14" -gt "15" ]
then 
    echo "大于"
elif [ "14" -lt "15" ]
then
    echo "小于"
else 
    echo "其他"
fi
