/*************************************************************************
	> File Name: sort.c
	> Author: wang
	> Mail: dhkhl235@gmail.com
	> Created Time: 2018年02月14日 星期三 21时56分20秒
 ************************************************************************/
/*  处理排序的函数 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define N 1000

void sort(int a[])
{
    for(int i = 0; i < N; i++)
    {
        for(int j = i+1; j < N; j++)
        {
            if(a[i] > a[j])
            {
                int temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
    }

}
int main()
{

    int arr[1024];
    
    int i = 0;
    while(i < N)
    {
        fscanf(stdin, "%d", &arr[i]);
        i++;
    }

    sort(arr);
    for(int i = 0; i < N; i++)
    printf("%d\n", arr[i]);

    
    return 0;
}
