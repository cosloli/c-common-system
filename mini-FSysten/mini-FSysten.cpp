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
#define OP_LEN 128
/*宏定义*/

/*全局变量*/
FILE *fp;
char mini_path[100] = "D:\\";
/*全局变量*/

/*指令函数定义*/
void attr(char *);
void cp(char*, char*);
void del(char* path);
void cd(char* path);
void create(char *name);
int format();
int mount(char arr[]);
int input(char* op1, char* op2, char* op3);
void move(char *path1, char *path2);
void mkdir(char* path);
void help(char * order);
void readtxt(char *);
void more(char *);
/*函数定义*/

/*功能函数定义*/
int analyse_path(char *str, char *name, char *expan_name, int flag1);

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
	_putw(0, fp);
	fclose(fp);
	//判断文件是否被创建
	if ((fp = fopen(mini_path, "r+")) != NULL)
		printf("创建文件成功！\n");
	fclose(fp);
	//Format();
}

/*format 1G space*/
int format() {
	if (strcat(mini_path, "D:\\") == 0)
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
	return 0;
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
	fread(&head, sizeof(superBlock), 1, fp);		//装载系统块
	rewind(fp);
	fseek(fp, 64 * head.freefcb_number, 0);
	fread(&fcb[head.freefcb_number], sizeof(F_CB), 1, fp);
	fseek(fp, 64 * head.freeib_number, 0);
	fread(&ib[head.freeib_number], sizeof(I_B), 1, fp);
	fseek(fp, 64 * head.fullfcb_number, 0);
	fread(&fcb[head.fullfcb_number], sizeof(F_CB), 1, fp);
	return 0;
}

void attr(char *)
{

}
void cp(char*, char*)
{

}
void del(char* path)
{

}
void cd(char* path)
{

}
void move(char *path1, char *path2)
{

}
void mkdir(char* path)
{

}
void help(char * order)
{

}
void readtxt(char *)
{

}
void more(char *)
{

}

/********************************  判断命令函数 ************************************/
void judge(char *op1, char *op2, char *op3)
{
	if (strcmp(op1, "create") == 0)
	{
		if (op2[0] == '\0')
			printf("命令语法不正确。\n");
		else if (strlen(op3) != 0)
			printf("命令语法不正确。\n");
		else
			create(op2);
	}
	else if (strcmp(op1, "mount") == 0)
	{

		if (op2[0] == '\0')
			printf("命令语法不正确。\n");
		else if (strlen(op3) != 0)
			printf("命令语法不正确。\n");
		else
			mount(op2);
	}
	else if (strcmp(op1, "cp") == 0)
	{
		if (op2[0] == '\0' || op3[0] == '\0')
			printf("命令语法不正确。\n");
		else
			cp(op2, op3);
	}
	else if (strcmp(op1, "fmt") == 0)
	{

		if (strlen(op3) != 0)
			printf("命令语法不正确。\n");
		else
			format();
	}
	else if (strcmp(op1, "cd") == 0)
	{
		if (strlen(op3) != 0)
			printf("命令语法不正确。\n");
		else
			cd(op2);
	}
	else if (strcmp(op1, "dl") == 0)
	{
		if (op2[0] == '\0')
			printf("命令语法不正确。\n");
		else if (strlen(op3) != 0)
			printf("命令语法不正确。\n");
		else
			del(op2);  //1
	}
	else if (strcmp(op1, "tp") == 0)
	{
		if (op2[0] == '\0')
			printf("命令语法不正确。\n");
		else if (strlen(op3) != 0)
			printf("命令语法不正确。\n");
		else
			readtxt(op2);   //1
	}
	else if (strcmp(op1, "more") == 0)
	{
		if (op2[0] == '\0')
			printf("命令语法不正确。\n");
		else if (strlen(op3) != 0)
			printf("命令语法不正确。\n");
		else
			more(op2);  //1
	}
	else if (strcmp(op1, "attr") == 0)
	{
		if (op2[0] == '\0')
			printf("命令语法不正确。\n");
		else if (strlen(op3) != 0)
			printf("命令语法不正确。\n");
		else
			attr(op2);   //1
	}
	else if (strcmp(op1, "help") == 0)
	{
		if (strlen(op3) != 0)
			printf("命令语法不正确。\n");
		help(op2);  //0
	}
	else if (strcmp(op1, "mkdir") == 0)
	{
		if (op2[0] == '\0')
			printf("命令语法不正确。\n");
		else if (strlen(op3) != 0)
			printf("命令语法不正确。\n");
		else
			mkdir(op2);   //1
	}
	else if (strcmp(op1, "mv") == 0)
	{
		if (op2[0] == '\0')
			printf("命令语法不正确。\n");
		else
			move(op2, op3);   //1
	}
	else
	{
		printf("输入指令有有误，请重新输入：\n");
	}
}


/********************************  输入命令函数 ************************************/
int input(char* op1, char* op2, char* op3)
{
	char op[OP_LEN+1];
	int flag[100];
	int i = 0, j = 0;
	int flag1 = 0;
	memset(flag, 0, sizeof(flag));
	for (i = 0;; i++)
	{
		if (i == OP_LEN)
		{
			if (flag1 == 0)
			{
				printf("输入命令字符过长，请输入%d以内的命令\n", i);
				flag1 = 1;
			}
			i = 0;
		}
		op[i] = getchar();
		if (op[i] == '\n')
		{
			if (flag1 == 1)
				return -1;
			op[i] = '\0';
			break;
		}
	}
	if (op[0] != ' ')
		flag[j++] = 0;
	for (i = 0; op[i] != '\0'; i++)
	{
		if ((op[i] == ' '&&op[i + 1] != ' ') || (op[i] != ' ' && (op[i + 1] == ' ' || op[i + 1] == '\0')))
			flag[j++] = i + 1;
	}
	for (i = flag[0]; i<flag[1]; i++)
		op1[i - flag[0]] = op[i];
	op1[i - flag[0]] = '\0';
	for (i = flag[2]; i<flag[3]; i++)
		op2[i - flag[2]] = op[i];
	op2[i - flag[2]] = '\0';
	for (i = flag[4]; i<flag[5]; i++)
		op3[i - flag[4]] = op[i];
	op3[i - flag[4]] = '\0';
	int len;
	len = strlen(op);
	if ((flag[5] != 0) && flag[5]<len)
	{
		printf("命令语法不正确。\n");
		return -1;
	}
	return 0;
}
/***********************************************************************************/
/********************************  输入命令函数 ************************************/
/***********************************************************************************/

int main()
{
	char op1[OP_LEN], op2[OP_LEN], op3[OP_LEN];
	char name[10] = "miniFS";
	printf("欢迎来到commonSystem");
	while (true)
	{
		printf("\ncommon>");
		
		if (input(op1, op2, op3) == -1)
		{
			continue;
		}
		if (strcmp(op1, "close")==0)
		{
			break;
		}
		judge(op1, op2, op3);
	}
}
