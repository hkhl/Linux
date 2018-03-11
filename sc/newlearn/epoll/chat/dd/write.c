/*************************************************************************
	> File Name: write.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月06日 星期三 01时16分41秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

int main()
{

    FILE *pf = fopen("./name.txt", "a+");
    if(pf == NULL)
    return 0;

    char buf[100] = "asdasd";
    fprintf(pf, "%s\n", buf);
    fprintf(pf, "12312312\n");




    char name[10][20];

    char uf[20];
    int n = 0, i = 0;
    FILE *pfr = fopen("./name.txt", "r");

    while(!feof(pfr))
    {
        fscanf(pfr, "%s\n", uf);
        printf("%s\n", uf);
    }
    fclose(pf);
    fclose(pfr);

    return 0;
}
