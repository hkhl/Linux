#!/bin/bash


echo "脚本的名字:$0"
echo "变量1:$1"
echo "变量2:$2"
echo "变量3:$3"
echo "变量4:$4"
echo "变量5:$5"
echo "变量6:$6"
echo "当前传递的参数个数: $#"
echo '$*'"的作用:$*"
echo "脚本当前进程ID号: $$"
echo "后台运行最后一个进程的进ID:$!"
echo "当前退出状态:$?"
echo "显示shell使用的当前选项,同set:$-"
