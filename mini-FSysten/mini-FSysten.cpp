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
#define OP_LEN 128			//每次输入指令的最大长度
/*宏定义*/


/*全局变量*/
FILE *fp;
char mini_path[100] = "D:\\";
/*全局变量*/

/*指令函数定义*/
int format();
int mount(char arr[]);
int input(char* op1, char* op2, char* op3);
void attr(char *);
void cp(char*, char*);
void del(char* path);
void cd(char* path);
void dr(char* path);
void create(char *name);
void judge(char* op1, char* op2, char* op3);
void move(char *path1, char *path2);
void mkdir(char* path);
void help(char * order);
void readtxt(char *);
void more(char *);
/*函数定义*/


/*功能函数定义*/
char *Getnowtime(char last_write_time[]);
int analyse_path(char *str, char *name, char *expan_name, int flag1);
int cp_s2c(char path1[], char path2[]);
int cp_c2s(char path1[], char path2[]);
int cp_c2c(char path1[], char path2[]);
/*功能函数定义*/


/*超级块结构*/
typedef struct Super_block {
	int freefcb_number = 0;		//空闲控制单元的数组下标
	int fullfcb_number = 0;		//根目录
	int freeib_number = 0;		//首个空闲文件信息块的“编号”（4033，262144）
}superBlock;
superBlock head;

/*控制单元结构*/
typedef struct FCB {
	int create_date;		//文件创建时间
	int edit_date;			//文件更新时间
	int dir_size;			//文件大小
	int file_num;			//文件头块编号
	int filep[7];			//子目录编号
	char filename[17];		//文件名字
	char is_delete;			//删除标记
	char file;				//文件类型
}F_CB;
F_CB fcb[FCB_NUMBER];

/*文件单元结构*/
typedef struct IB {
	int block_num;			//自己的块号
	int length;				//持续长度
	int last_num;			//前面块头的块号
	int next_num;			//后面块头的块号
}I_B;
I_B ib[IB_NUMBER];

/*Creat 1G space*/
void create(char *name)
{
	int i;
	for (i = 0; name[i] != '\0'; i++)
	{
		if (!((name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z') || (name[i] >= '0' && name[i] <= '9')))
		{
			printf("System name input is not legal!\n");
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
		printf("System name input is not legal!\n");
		return;
	}
	//开辟1G空间
	fseek(fp, 1024 * 1024 * 1024, 0);
	_putw(0, fp);
	fclose(fp);
	//判断文件是否被创建
	if ((fp = fopen(mini_path, "r+")) != NULL)
		printf("\nCreate File Successfully!\n");
	fclose(fp);
	format();
}

/*Format 1G space*/
int format() {
	if (strcmp(mini_path, "D:\\") == 0)
	{
		printf("please create a mini_FS first!!!\n");
		return -1;
	}
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("format failed!!!\n");
		return -1;
	}

	//系统块初始化
	head.freefcb_number = 1;
	head.freeib_number = 0;

	//空文件头初始化
	ib[0].block_num = 0;
	ib[0].length = 258111;
	ib[0].last_num = -1;		//-1表示此为空文件信息块头
	ib[0].next_num = -1;		//-1表示此为最后一段空文件信息段

	//根目录初始化
	strcat(fcb[0].filename, "home");	//根目录文件名
	time_t createhometime = 0;	//获取时间
	createhometime = time(NULL);
	fcb[0].create_date = createhometime;
	fcb[0].edit_date = createhometime;
	fcb[0].file = 0;			//文件夹
	fcb[0].dir_size = 0;		//空文件夹，大小为0
	fcb[0].is_delete = 0;		//未删除
	fcb[0].file_num = -1;		//文件夹
	for (int i = 0; i < 7; i++)
		fcb[0].filep[i] = -1;	//暂时不存在根目录
	return 0;
}

/*Input orders!!!*/
int input(char* op1, char* op2, char* op3)
{
	char op[OP_LEN + 1];
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
				printf("The input command character is too long!! Please enter the command within %d.\n", i);
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
	for (i = flag[0]; i < flag[1]; i++)
		op1[i - flag[0]] = op[i];
	op1[i - flag[0]] = '\0';
	for (i = flag[2]; i < flag[3]; i++)
		op2[i - flag[2]] = op[i];
	op2[i - flag[2]] = '\0';
	for (i = flag[4]; i < flag[5]; i++)
		op3[i - flag[4]] = op[i];
	op3[i - flag[4]] = '\0';
	int len;
	len = strlen(op);
	if ((flag[5] != 0) && flag[5] < len)
	{
		printf("Command syntax is incorrect\n");
		return -1;
	}
	return 0;
}

/*Judge users' order*/
void judge(char *op1, char *op2, char *op3)
{
	//Create Command
	if (strcmp(op1, "create") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect\n");
		else
			create(op2);
	}

	//Mount Command
	else if (strcmp(op1, "mount") == 0)
	{

		if (op2[0] == '\0')
			printf("Command syntax is incorrect\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect\n");
		else
			mount(op2);
	}

	//Copy Command
	else if (strcmp(op1, "cp") == 0)
	{
		if (op2[0] == '\0' || op3[0] == '\0')
			printf("Command syntax is incorrect\n");
		else
			cp(op2, op3);
	}

	//Format Command
	else if (strcmp(op1, "fmt") == 0)
	{

		if (strlen(op2) != 0)
			printf("Command syntax is incorrect\n");
		else
			format();
	}

	//Show Contents Command
	else if (strcmp(op1, "dr") == 0)
	{
		if (strlen(op3) != 0)
			printf("Command syntax is incorrect\n");
		else
			dr(op2);
	}

	//Enter Contents Command
	else if (strcmp(op1, "cd") == 0)
	{
		if (strlen(op3) != 0)
			printf("Command syntax is incorrect\n");
		else
			cd(op2);
	}

	//Delete Command
	else if (strcmp(op1, "dl") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect\n");
		else
			del(op2);  //1
	}

	//Show Text Command
	else if (strcmp(op1, "tp") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect\n");
		else
			readtxt(op2);   //1
	}

	//Paging Show Text Command
	else if (strcmp(op1, "more") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect\n");
		else
			more(op2);  //1
	}

	//Show File Attribute Command
	else if (strcmp(op1, "attr") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect\n");
		else
			attr(op2);   //1
	}

	//Show Help Command
	else if (strcmp(op1, "help") == 0)
	{
		if (strlen(op3) != 0)
			printf("Command syntax is incorrect\n");
		help(op2);  //0
	}

	//Create Content Command
	else if (strcmp(op1, "mkdir") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect\n");
		else
			mkdir(op2);   //1
	}

	//Move Command
	else if (strcmp(op1, "mv") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect\n");
		else
			move(op2, op3);   //1
	}

	else
	{
		printf("The input command is incorrect!Please re-enter...\n");
	}
}

/*Mount used Mini_FS*/
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
	//装载系统块
	rewind(fp);
	fread(&head, sizeof(superBlock), 1, fp);
	//装载空闲控制单元头
	rewind(fp);
	fseek(fp, 64 * head.freefcb_number, 0);
	fread(&fcb[head.freefcb_number], sizeof(F_CB), 1, fp);
	//装载空闲信息块头
	fseek(fp, 64 * head.freeib_number, 0);
	fread(&ib[head.freeib_number], sizeof(I_B), 1, fp);
	//装载根目录
	fseek(fp, 64 * head.fullfcb_number, 0);
	fread(&fcb[head.fullfcb_number], sizeof(F_CB), 1, fp);

	printf("mount succeed!!!\n");
	fclose(fp);
	return 0;
}

void attr(char *)
{

}


/***********************************************************************************/
/************************  复制函数主函数 ******************************************/
/***********************************************************************************/
void cp(char path1[], char path2[])
{
	int len1, len2;
	len1 = strlen(path1);
	len2 = strlen(path2);
	if (len1 < 3 || len2 < 3)
	{
		printf("输入指令格式有错，请核对后重新输入！\n");
		return;
	}
	if (!((path1[len1 - 2] == '.' || path1[len1 - 3] == '.' || path1[len1 - 4] == '.') && (path2[len2 - 2] == '.' || path2[len2 - 3] == '.' || path2[len2 - 4] == '.')))
	{
		printf("输入指令格式有错，请核对后重新输入！\n");
		return;
	}
	if (path1[1] == ':'&&path1[2] == '\\')
	{
		if (path1[4] == '\\' || path1[3] != '\\')
		{
			printf("输入指令格式有错，请核对后重新输入！\n");
			return;

		}
		cp_s2c(path1, path2);

	}
	else if (path2[1] == ':'&&path2[2] == '\\')
	{
		if (path2[4] == '\\' || path2[3] != '\\')
		{
			printf("输入指令格式有错，请核对后重新输入！\n");
			return;
		}
		cp_c2s(path1, path2);
	}
	else if (path1[1] == ':'&&path1[2] == '\\'&&path1[3] == '\\'&&path2[1] == ':'&&path2[2] == '\\'&&path2[3] == '\\')
	{
		printf("输入指令格式有错，请核对后重新输入！\n");
		return;
	}
	else  cp_c2c(path1, path2);
	return;
}
/***********************************************************************************/
/************************  复制函数主函数 ******************************************/
/***********************************************************************************/
void del(char* path)
{

}

void cd(char* path)
{

}

/*Show the contents for users*/
void dr(char* path) 
{

}

void move(char *path1, char *path2)
{

}

void mkdir(char* path)
{

}

/*Show help to users*/
void help(char * order)
{
	char ch;
	if ((fp = fopen("D:\\help.txt", "rb+")) == NULL)
	{
		printf("Open help file error\n");
		getchar();
		exit(1);
	}
	if (order[0] == '\0')
	{
		ch = fgetc(fp);
		while (ch != EOF && ch != '1')
		{
			if (ch >= 'A'&&ch <= 'Z')
				ch = ch + 'a' - 'A';
			putchar(ch);
			ch = fgetc(fp);
		}
		fclose(fp);
	}
	else
	{
		char info[300];
		while (1)
		{
			fscanf(fp, "%s", info);
			if ((strcmp("end", info)) == 0)
				break;
			if ((strcmp(order, info)) == 0)
			{
				while (1)
				{
					fgets(info, 300, fp);
					if (info[0] == '1')
						break;
					printf("%s", info);
				}
				fclose(fp);
				return;
			}
		}
		printf("The input command is incorrect. Please re-enter!\n");
		fclose(fp);
	}
}

void readtxt(char *)
{

}

void more(char *)
{

}

int main()
{
	char op1[OP_LEN];
	char op2[OP_LEN];
	char op3[OP_LEN];
	printf("Welcome to COMMON Mini_File_System!\n");
	printf("\nGet help, please enter help!\n");
	printf("\n\nIf you are familiar with the operation of this system, please ignore the second sentence!\n\n\n");
	printf("\nEnter the command character to operate, enter \"close\" to exit operation\n");
	while (1)
	{
		printf("\nMini_FS>");
		if ((input(op1, op2, op3) == -1))
			continue;
		if (strcmp(op1, "close") == 0)
			break;
		judge(op1, op2, op3);
	}
	printf("\nThank you for using COMMON Mini_File_System!\n");
}
