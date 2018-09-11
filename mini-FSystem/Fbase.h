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
	int last_ib;				//���������Ϣ��
	int last_fcb;				//��������ļ����ƿ�
	int freefcb_id;				//��һ�����п��Ƶ�Ԫλ�� (0,258047)
	int rootfcb_id;				//��Ŀ¼λ��
	int freeib_id;				//��һ��������Ϣ��λ�ã�4033��262144��
	int last_write_time;		//���һ��д��ʱ��
}superBlock;

/*���Ƶ�Ԫ�ṹ*/
typedef struct F_CB {
	int create_time;		//�ļ�����ʱ��
	int edit_time;			//�ļ�����ʱ��
	int file_size;			//�ļ�ռ�ÿ���
	int file_block_id;		//�ļ�ͷ����
	int filep[FCB_WIDTH];			//��Ŀ¼�������
	char filename[13];		//�ļ�����
	char is_delete;			//ɾ�����
	char file_type;			//�ļ�����
}FCB;

/*��չ���Ƶ�Ԫ�ṹ*/
typedef struct ext_CB {
	int filep[16];			//��Ŀ¼���
}extCB;

/*�ļ���Ԫ�ṹ*/
typedef struct I_B {
	int block_num;			//�Լ��Ŀ��
	int length;				//��������
	int last_num;			//ǰ���ͷ�Ŀ��
	int next_num;			//�����ͷ�Ŀ��
}IB;

/*ȫ�ֱ���*/
extern FILE *fp;
extern char mini_path[100];
extern int current_fcb_id;
extern int sys_mounted;
extern superBlock sys;
extern FCB fcb_list[FCB_NUM];
extern IB ib_tmp[2];
/*ȫ�ֱ���*/

using namespace std;

#endif // !F_BASE_H


