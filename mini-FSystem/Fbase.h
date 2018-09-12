#pragma once
#ifndef F_BASE_H
#define F_BASE_H
#include "pch.h"
#include <iostream>
#include <windows.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

/*������ṹ*/
typedef struct Super_block {
	int fcb_Location;			//��һ���ļ����ƿ��λ��
	int ib_location;			//��һ�����ݿ��λ��
	int fcb_size;				//fcb��С
	int blk_size;				//���С
	int superblock_size;		//�������С
	int superblock_location;	//������λ��
	int last_write_ib;			//���д�����Ϣ��
	int last_write_fcb;			//���д����ļ����ƿ�
	int freefcb_id;				//��һ�����п��ƿ�λ�� (0,258047)
	int freeib_id;				//��һ��������Ϣ��λ�ã�4033��262144��
	int last_freefcb_id;		//���һ�����п��ƿ�λ��
	int last_freeib_id;			//���һ��������Ϣ��λ��
	int rootfcb_id;				//��Ŀ¼λ��
	int last_write_time;		//���һ��д��ʱ��
}superBlock;

/*���Ƶ�Ԫ�ṹ*/
typedef struct F_CB {
	int create_time;		//�ļ�����ʱ��
	int edit_time;			//�ļ�����ʱ��
	int file_size;			//�ļ�ռ�ÿ���
	int file_block_id;		//�ļ�ͷ����
	int filep[FCB_WIDTH];			//��Ŀ¼�������
	char filename[14];		//�ļ�����
	char file_type;			//�ļ�����
}FCB;

/*��չ���Ƶ�Ԫ�ṹ*/
typedef struct ext_CB {
	int filep[16];			//��Ŀ¼���
}extCB;

/*�ļ���Ԫ�ṹ*/
typedef struct FREE_I_B {
	int block_id;			//�Լ��Ŀ��
	int length;				//��������
	int last_block;			//ǰ���ͷ�Ŀ��
	int next_block;			//�����ͷ�Ŀ��
	int left;				//��С��
	int right;				//�����
}FreeIB;

typedef struct FULL_I_B {
	int block_id;
	int lenght;
	int next_block;
}FullIB;

/*ȫ�ֱ���*/
extern FILE *fp;
extern char mini_path[100];
extern int current_fcb_id;
extern int sys_mounted;
extern char current_path[128];
extern superBlock sys;
extern FCB fcb_list[FCB_NUM];
extern FreeIB ib_tmp[2];
/*ȫ�ֱ���*/

using namespace std;

#endif // !F_BASE_H


