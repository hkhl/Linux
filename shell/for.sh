#!/bin/bash

#第一版本
for i in  1 2 3 4 5
do 
    echo -e "$i\n" 
done

#第二版本
for i in "aaa bbb  ccc"
do 
    echo -e "$i\n" 
done


#第三版本
for i in aaa vvv ccc
do 
    echo -e "$i\n" 
done




#第四版本  in 后加命令 反引号  
#文件中以空格和 \n为结束符号
echo "第四版本"
for i in `cat myfile.txt`
do
    echo "$i"
done


#第五版本
i=1
for param
do 
    echo "param #$I is $param"   #$I可有可无,,  命令行参数都给就可以?
    i=$i+1;      #这句话可有可无
done




