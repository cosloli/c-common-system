// mini-FSysten.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

using namespace std;

/*宏定义*/
#define BLOCK_SIZE 4096		//每个块的大小
#define BLOCK_NUMBER 262144 //文件系统中块的数量
#define FCB_NUMBER 4032 * 64//控制单元的数量为258048个，隶属于（1，4032），寻找控制块直接访问该块所在数组(0,258047)
#define IB_NUMBER 258111	//文件信息块数量为258111个，隶属于（4033，262144）
/*宏定义*/

/*全局变量*/
FILE *fp;
char mini_path[100] = "E:\\";
/*全局变量*/

/*函数定义*/
void create(char *name);
int format();
int mount(char arr[]);
/*函数定义*/

/*超级块结构*/
typedef struct Super_block {
	int freefcb_number = 0;		//空闲控制单元的数组下标
	int fullfcb_number = 0;		//根目录
	int freeib_number = 0;		//首个空闲文件信息块的“编号”（4033，262144）
}superBlock;
superBlock head;
/*超级块结构*/

/*控制单元结构*/
typedef struct FCB {
	char filename[15];		//文件名字
	int create_date;		//文件创建时间
	int edit_date;			//文件更新时间
	int dir_size;			//文件大小
	char is_delete;			//删除标记
	int file_num;			//文件头块编号
	/*八个子文件、后面3 * 8位比特，24位表示文件下标；最后一个为扩展单元，保存扩展单元的数组下标*/
	char filep[9][3];
}F_CB;
F_CB fcb[FCB_NUMBER];

/*文件单元结构*/
typedef struct IB {
	int block_num;			//自己的块号
	int length;				//持续长度
	int last_num;			//前面空闲块头的块号
	int next_num;			//后面空闲块头的块号
}I_B;
I_B ib[IB_NUMBER];

/*creat 1G space*/
void create(char *name)
{
	int i;
	for (i = 0; name[i] != '\0'; i++)
	{
		if (!((name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z') || (name[i] >= '0' && name[i] <= '9')))
		{
			printf("系统名称输入不合法！\n");
			return;
		}
	}
	//文件系统所在路径
	mini_path[3] = '\0';
	strcat(mini_path, name);

	/*.DAT并不是一种标准文件。许多软件都使用这个扩展名，但文件含义不同。
		DAT文件，可以按照扩展名来看就是DATA的意思，即数据文件，这类文件并没有进行绝对化的定义。*/
	strcat(mini_path, ".dat");

	//以读/写方式打开或建立一个二进制文件，允许读和写。（为NULL则打不开）
	if ((fp = fopen(mini_path, "wb+")) == NULL)
	{
		printf("系统名称输入不合法!\n");
		return;
	}
	//开辟1G空间
	fseek(fp, 1024 * 1024 * 1024, 0);
	putw(0, fp);
	fclose(fp);
	//判断文件是否被创建
	if ((fp = fopen(mini_path, "r+")) != NULL)
		printf("创建文件成功！\n");
	fclose(fp);
	//Format();
}

/*format 1G space*/
int format() {
	if (strcat(mini_path, "E:\\") == 0)
	{
		printf("please create a mini_FS first!!!\n");
		return -1;
	}
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("format failed!!!\n");
		return -1;
	}
	head.freefcb_number = 0;
	head.freeib_number = 0;
}

int mount(char arr[])
{
	mini_path[3] = '\0';
	strcat(mini_path, arr);
	strcat(mini_path, ".dat");
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("mount failed!!!\n");
		return -1;
	}
	rewind(fp);
	fread(&head, sizeof(superBlock), 1, fp);
	int freefcbnum = head.freefcb_number;
	int freeibnum = head.freeib_number;
	int fullfcbnum = head.fullfcb_number;
	rewind(fp);
	fseek(fp, 64 * freefcbnum, 0);
	fread(&fcb[freefcbnum], sizeof(F_CB), 1, fp);
	fseek(fp, 64 * freeibnum, 0);
	fread(&ib[freeibnum], sizeof(I_B), 1, fp);
	fseek(fp, 64 * fullfcbnum, 0);
	fread(&fcb[fullfcbnum], sizeof(F_CB), 1, fp);
}

int main()
{
	char name[10] = "miniFS";
	create(name);
}
