#include "pch.h"
#include "Fbase.h"
/*ȫ�ֱ���*/
FILE *fp;
char mini_path[100] = "D:\\";
int current_fcb_id;
int sys_mounted = 0;
superBlock sys;
FCB fcb_list[FCB_NUM];
IB ib_tmp[2];