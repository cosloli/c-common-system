#include "pch.h"
#include "Fcommand.h"

/***********************************************************************************/
/************************  ���ƺ��������� ******************************************/
/***********************************************************************************/
void cp(char path1[], char path2[])
{
	int len1, len2;
	int dir_fcb_id;
	len1 = strlen(path1);
	len2 = strlen(path2);
	if (len1 < 3 && len2 < 3)
	{
		printf("Path format incorrect. Please re-enter.\n");
		return;
	}
	/*
	if (!((path1[len1] == '.' || path1[len1 - 3] == '.' || path1[len1 - 4] == '.') && (path2[len2 - 2] == '.' || path2[len2 - 3] == '.' || path2[len2 - 4] == '.')))
	{
		printf("����ָ���ʽ�д���˶Ժ��������룡\n");
		return;
	}*/
	if (path1[1] == ':'&&path1[2] == '\\')
	{
		make(path1, FILE_T, path2);
	}
	return;
}

int cp_s2c(char path1[], char path2[])
{
	return 0;
}
int cp_c2s(char path1[], char path2[])
{
	return 0;
}
int cp_c2c(char path1[], char path2[])
{

	return 0;
}
