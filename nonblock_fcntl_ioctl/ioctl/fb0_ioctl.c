/*************************************************************************
	> File Name: fb0_ioctl.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月14日 星期日 11时14分47秒
************************************************************************/

/* 屏幕分辨率 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>

int main()
{
    int fd;
    struct fb_var_screeninfo vinfo;
    fd = open("/dev/fb0", O_RDONLY);
    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
    {
        perror("ioctl");
        exit(1);
    }
    printf("r:%d\nc:%d\n", vinfo.xres, vinfo.yres);
    close(fd);

    return 0;
}
