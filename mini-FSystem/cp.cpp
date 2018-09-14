#include "pch.h"
#include "Fcommand.h"

/***********************************************************************************/
/************************  ���ƺ��������� ******************************************/
/***********************************************************************************/
void cp(char path1[], char path2[])
{
	int dir_fcb_id;
	FILE* fp_tmp;
	IB_Disk* ib;
	char *buf;
	int f_len;
	if (path2[2] != '\\')
	{
		make(path2, FILE_T, path1);
	}
	else
	{
		dir_fcb_id = path_to_fcb_id(path1, FILE_T);
		switch (dir_fcb_id)
		{
		case -2:
		case -1:
			return;
		default:
			break;
		}
		ib = get_ib_info(fcb_list[dir_fcb_id].file_block_id);
		//�Զ�/д��ʽ�򿪻���һ���ı��ļ����������д����ΪNULL��򲻿���
		if ((fp_tmp = fopen(path2, "wb+")) == NULL)
		{
			printf("Name illegal!\n");
			return;
		}
		buf = new char[ib->size];
		fseek(fp, IB_POS(ib->block_id), SEEK_SET);
		fread(buf, ib->size, 1, fp);
		fwrite(buf, sizeof(buf), 1, fp_tmp);
		fclose(fp_tmp);
		delete ib;
	}
	return;
}
