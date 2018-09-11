#include "pch.h"
#include "Fbase.h"
#include "Ftools.h"
#include "Fcore.h"

using namespace std;

/*����fcb��*/
int new_fcb(int dir_fcb_id, char fcb_type, char *name, FILE* fp_tmp)
{
	int free_id;
	int child;
	int fcb_id_tmp = dir_fcb_id;
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		cout << "New fcb failed." << endl;
		return -2;
	}
	/*������*/
	for (child = 2; fcb_list[fcb_id_tmp].filep[child] != -1; child++)
	{
		if (strcmp(name, fcb_list[fcb_list[fcb_id_tmp].filep[child]].filename) == 0)
		{
			cout << "Name duplicated. Please re-enter." << endl;
			return -1;
		}
		if (child == EXT_CB)
		{
			fcb_id_tmp = fcb_list[fcb_id_tmp].filep[child];
			child = 1;
		}
	}
	/*����հ�FCB*/
	free_id = sys.freefcb_id;
	fcb_list[fcb_id_tmp].filep[child] = free_id;
	sys.freefcb_id = fcb_list[free_id].filep[0];
	sys.last_fcb = free_id;
	fcb_list[fcb_id_tmp].edit_time = current_time();
	/*��ʼ���հ�FCB*/
	fcb_list[free_id].file_type = DIR_T;
	fcb_list[free_id].create_time = current_time();
	fcb_list[free_id].edit_time = fcb_list[free_id].create_time;
	fcb_list[free_id].is_delete = 0;
	fcb_list[free_id].filep[0] = fcb_id_tmp;
	fcb_list[free_id].filep[1] = free_id;
	/*дӲ��*/
	fseek(fp, FCB_Location * BLOCK_SIZE + free_id * FCB_SIZE, SEEK_SET);
	fwrite(&fcb_list[free_id], sizeof(FCB), 1, fp);
	fseek(fp, FCB_Location * BLOCK_SIZE + fcb_id_tmp * FCB_SIZE, SEEK_SET);
	fwrite(&fcb_list[fcb_id_tmp], sizeof(FCB), 1, fp);
	fflush(fp);
	update_sys();
	/*ת���հ�FCB*/
	fcb_id_tmp = free_id;
	if (child == EXT_CB)
	{
		//���������
		fcb_list[fcb_id_tmp].file_type = EXT_T;
		//����հ�FCB
		free_id = sys.freefcb_id;
		fcb_list[fcb_id_tmp].filep[1] = free_id;//��չ����±�1��ʼ����
		sys.freefcb_id = fcb_list[free_id].filep[0];
		sys.last_fcb = free_id;
		//��ʼ���հ�FCB
		fcb_list[free_id].file_type = DIR_T;
		fcb_list[free_id].create_time = current_time();
		fcb_list[free_id].edit_time = fcb_list[free_id].create_time;
		fcb_list[free_id].is_delete = 0;
		fcb_list[free_id].filep[0] = fcb_id_tmp;
		fcb_list[free_id].filep[1] = free_id;
		strcpy(fcb_list[free_id].filename, name);
		/*дӲ��*/
		fseek(fp, FCB_Location * BLOCK_SIZE + free_id * FCB_SIZE, SEEK_SET);
		fwrite(&fcb_list[free_id], sizeof(FCB), 1, fp);
		fseek(fp, FCB_Location * BLOCK_SIZE + fcb_id_tmp * FCB_SIZE, SEEK_SET);
		fwrite(&fcb_list[fcb_id_tmp], sizeof(FCB), 1, fp);
		fflush(fp);
		update_sys();
		/*ת���հ�FCB*/
		fcb_id_tmp = free_id;
	}
	else
	{
		strcpy(fcb_list[fcb_id_tmp].filename, name);
		fseek(fp, FCB_Location * BLOCK_SIZE + fcb_id_tmp * FCB_SIZE, SEEK_SET);
		fwrite(&fcb_list[fcb_id_tmp], sizeof(FCB), 1, fp);
		fflush(fp);
	}
	if (fcb_type == FILE_T)
	{
		fcb_list[fcb_id_tmp].file_type = FILE_T;
		fseek(fp, FCB_Location * BLOCK_SIZE + fcb_id_tmp * FCB_SIZE, SEEK_SET);
		fwrite(&fcb_list[fcb_id_tmp], sizeof(FCB), 1, fp);
	}
	//cout << ftell(fp) << endl;

	fflush(fp);
	return 0;
}

int update_fcb(int dir_fcb_id, int fcb_id, char *name, FILE* fp_tmp)
{
	int child;
	int fcb_id_tmp = dir_fcb_id;
	int origin_fcb_id;
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		cout << "Update fcb failed." << endl;
		return -2;
	}
	/*��������������·��*/
	for (child = 2; fcb_list[fcb_id_tmp].filep[child] != -1; child++)
	{
		if (strcmp(name, fcb_list[fcb_list[fcb_id_tmp].filep[child]].filename) == 0)
		{
			cout << "Name duplicated. Please re-enter." << endl;
			return -1;
		}
		if (child == EXT_CB)
		{
			fcb_id_tmp = fcb_list[fcb_id_tmp].filep[child];
			child = 1;
		}
	}
	if (fcb_id_tmp != fcb_list[fcb_id].filep[0])
	{
		fcb_list[fcb_id_tmp].filep[child] = fcb_id;
		fcb_list[fcb_id].filep[0] = dir_fcb_id;
	}
	fcb_list[fcb_id].edit_time = current_time();
	strcpy(fcb_list[fcb_id].filename, name);
	if (fcb_list[fcb_id].file_type == FILE_T)
	{

	}
	/*д����*/
	fseek(fp, FCB_Location * BLOCK_SIZE + fcb_id * FCB_SIZE, SEEK_SET);
	fwrite(&fcb_list[fcb_id], sizeof(FCB), 1, fp);
	fflush(fp);
	return 0;
}

/*�ͷ�fcb��*/
int drop_fcb(int dir_fcb_id, int fcb_id)
{
	return 0;
}

void update_sys()
{
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Update failed.\n");
		return;
	}
	sys.last_write_time = current_time();
	fseek(fp, SUPER_Loacation * BLOCK_SIZE, SEEK_SET);
	fwrite(&sys, sizeof(superBlock), 1, fp);
	fflush(fp);
	return;
}