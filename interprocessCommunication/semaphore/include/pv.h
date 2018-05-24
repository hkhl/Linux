#ifndef __PV_H__
#define __PV_H__

//初始化sennums个信号灯/信号量的值value
extern int I(int semnums, int value);

//对信号量集(semid)中的(某个)信号灯(semnum)做P操作
extern void P(int semid, int semnum, int value);

//对信号量集(semid)中的信号灯(semnum)做V操作
extern void V(int semid, int semnum, int value);

//销毁信号量集(semid)
extern void D(int semid);


#endif
