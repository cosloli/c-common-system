#include "pch.h"
#include "Fcore.h"

/***************************************��ȡʱ�亯��*********************************/
/************************************************************************************/
int current_time()
{		                                         //nowtime:��ǰϵͳʱ��,last_write_time[15]:���д��ʱ�䣬�������յ�ǰϵͳʱ��
	int ntime;
	char sTime[20] = { 0 };						//�ú����������<time.h><string.h>ͷ�ļ�
	time_t ftime = time(NULL)/* + (8 * 3600)*/;
	ntime = (int)ftime;
	//tm* pTime = gmtime(&ftime);
	//strftime(sTime, 80, "%Y%m%d%H%M%S", pTime);//����1
	//cout << sTime << endl;
	//strcpy(datetime, sTime);
	return ntime;
}
/************************************************************************************/
/***************************************��ȡʱ�亯��*********************************/

/*·�� -> fcb���*/
int path_to_fcb_id(char *path, int f_type)
{
	char input_tmp[13] = { 0 };
	char name[100][13] = { 0 };
	char str[100];
	int len, abs = 0;
	int fcb_id = 0, next_id;
	int i = 0, j = 0, k = 0;

	strcpy(str, path);
	len = strlen(path);
	//���·��
	for (i = 0, j = 0, k = 0; i < len; i++)
	{
		if (str[i] == '\\')
		{
			if (i && str[i - 1] == '\\')
			{
				cout << "Path format incorrect. Please re-enter." << endl;
				return -2;
			}
			if (i == len - 1)
			{
				input_tmp[j] = '\0';
			}
			if (i == 0)
			{
				abs = 1;
			}
			else
			{
				strcpy(name[k++], input_tmp);
			}
			memset(input_tmp, 0, sizeof(input_tmp));
			j = 0;
		}
		else
		{
			input_tmp[j++] = str[i];
		}

	}
	if (j != 0)
	{
		strcpy(name[k++], input_tmp);
	}
	//��ʼ·����λ
	if (abs)
	{
		fcb_id = sys.rootfcb_id;
	}
	else
	{
		fcb_id = current_fcb_id;
	}
	//��������
	for (i = 0; i < k; i++)
	{
		if (name[i][0] == '.')
		{
			if (name[i][1] == '.' && name[i][2] == '\0')
			{
				fcb_id = fcb_list[fcb_id].filep[0];
				continue;
			}
			else if (name[i][1] == '\0')
			{
				continue;
			}
		}

		for (j = 2, next_id = fcb_list[fcb_id].filep[j]; next_id != -1; j++)
		{
			if (j == EXT_CB)
			{
				fcb_id = fcb_list[fcb_id].filep[j];
				j = 2;
			}
			if (strcmp(fcb_list[next_id].filename, name[i]) == 0)
			{
				if (f_type == DIR_T && fcb_list[next_id].file_type != DIR_T)
				{
					cout << "No such directory." << endl;
					return -1;
				}
				if (f_type == FILE_T && fcb_list[next_id].file_type != FILE_T)
				{
					cout << "No such file." << endl;
					return -1;
				}
				fcb_id = next_id;
				break;
			}
			next_id = fcb_list[fcb_id].filep[j];
		}
		if (next_id == -1)
		{
			if (f_type == DIR_T)
			{
				cout << "No such directory." << endl;
			}
			else if (f_type == FILE_T)
			{
				cout << "No such file." << endl;
			}
			else
			{
				cout << "No such directory or file." << endl;
			}
			return -1;
		}
	}
	return fcb_id;
}

/*����fcb��*/
int new_fcb(int dir_fcb_id, char fcb_type, char *name, char *file_path)
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
		if (child == EXT_CB)
		{
			fcb_id_tmp = fcb_list[fcb_id_tmp].filep[child];
			child = 1;
		}
		if (strcmp(name, fcb_list[fcb_list[fcb_id_tmp].filep[child]].filename) == 0)
		{
			cout << "Name duplicated. Please re-enter." << endl;
			return -1;
		}
	}
	/*����հ�FCB*/
	free_id = sys.freefcb_id;
	fcb_list[fcb_id_tmp].filep[child] = free_id;
	sys.freefcb_id = fcb_list[free_id].filep[0];
	sys.last_write_fcb = free_id;
	fcb_list[fcb_id_tmp].edit_time = current_time();
	/*��ʼ���հ�FCB*/
	fcb_list[free_id].file_type = DIR_T;
	fcb_list[free_id].create_time = current_time();
	fcb_list[free_id].edit_time = fcb_list[free_id].create_time;
	fcb_list[free_id].filep[0] = fcb_id_tmp;
	fcb_list[free_id].filep[1] = free_id;
	/*дӲ��*/
	update_fcb(free_id);
	update_fcb(fcb_id_tmp);
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
		sys.last_write_fcb = free_id;
		//��ʼ���հ�FCB
		fcb_list[free_id].file_type = DIR_T;
		fcb_list[free_id].create_time = current_time();
		fcb_list[free_id].edit_time = fcb_list[free_id].create_time;
		fcb_list[free_id].filep[0] = fcb_id_tmp;
		fcb_list[free_id].filep[1] = free_id;
		strcpy(fcb_list[free_id].filename, name);
		/*дӲ��*/
		update_fcb(free_id);
		update_fcb(fcb_id_tmp);
		update_sys();
		/*ת���հ�FCB*/
		fcb_id_tmp = free_id;
	}
	else
	{
		strcpy(fcb_list[fcb_id_tmp].filename, name);
		update_fcb(fcb_id_tmp);
	}
	if (fcb_type == FILE_T)
	{
		fcb_list[fcb_id_tmp].file_type = FILE_T;
		update_fcb(fcb_id_tmp);
	}
	fflush(fp);
	return 0;
}

/*�ƶ�FCB*/
int move_fcb(int dir_fcb_id, int fcb_id, char *name, char* file_path)
{
	int child;
	int tmp_fcb_id = dir_fcb_id;
	int origin_fcb_id = fcb_list[fcb_id].filep[0];
	int origin_child;
	int drop;
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		cout << "Update fcb failed." << endl;
		return -2;
	}
	if (name[0] == '\0')
	{
		cout << "Empty name. Please re-enter" << endl;
	}
	/*��������Ѱ����filepλ��*/
	for (child = 2; fcb_list[tmp_fcb_id].filep[child] != -1; child++)
	{
		if (child == EXT_CB)
		{
			tmp_fcb_id = fcb_list[tmp_fcb_id].filep[child];
			child = 1;
		}
		if (strcmp(name, fcb_list[fcb_list[tmp_fcb_id].filep[child]].filename) == 0)
		{
			cout << "Name duplicated. Please re-enter." << endl;
			return -1;
		}
	}
	/*���·��*/
	//fcb_list[fcb_id].filep[0] = 0;
	if (tmp_fcb_id != fcb_list[fcb_id].filep[0])
	{
		/*������·��*/
		fcb_list[tmp_fcb_id].filep[child] = fcb_id;
		fcb_list[fcb_id].filep[0] = dir_fcb_id;
		update_fcb(tmp_fcb_id);
		update_fcb(fcb_id);
		/*ɾ��ԭ��·��*/
		for (origin_child = 2; fcb_list[origin_fcb_id].filep[origin_child] != -1; origin_child++)
		{
			if (origin_child == EXT_CB)
			{
				origin_fcb_id = fcb_list[origin_fcb_id].filep[origin_child];
				origin_child = 1;
			}
			if (fcb_list[origin_fcb_id].filep[origin_child] == fcb_id)
			{
				//��ǰ�ƶ�filep
				for (drop = origin_child; fcb_list[origin_fcb_id].filep[drop] != -1; drop++)
				{
					if (drop == EXT_CB)
					{
						origin_fcb_id = fcb_list[origin_fcb_id].filep[drop];
						fcb_list[fcb_list[origin_fcb_id].filep[0]].filep[drop - 1] == fcb_list[origin_fcb_id].filep[1];
						update_fcb(origin_fcb_id);
						drop = 1;
					}
					fcb_list[origin_fcb_id].filep[drop] = fcb_list[origin_fcb_id].filep[drop + 1];
					if (fcb_list[origin_fcb_id].filep[drop] == -1)
					{
						break;
					}
				}
				if (drop == 1)
				{
					//�Ͽ�extCB����
					fcb_list[fcb_list[origin_fcb_id].filep[0]].filep[EXT_CB] = -1;
					update_fcb(fcb_list[origin_fcb_id].filep[0]);
					//�ͷ�extCB
					fcb_list[origin_fcb_id].filep[0] = sys.freefcb_id;
					update_fcb(origin_fcb_id);
					sys.freefcb_id = origin_fcb_id;
					update_sys();
				}
				else
				{
					update_fcb(origin_fcb_id);
				}
				break;
			}
		}

	}
	fcb_list[fcb_id].edit_time = current_time();
	strcpy(fcb_list[fcb_id].filename, name);
	if (fcb_list[fcb_id].file_type == FILE_T)
	{

	}
	/*д����*/
	update_fcb(fcb_id);
	return 0;
}

/*�ͷ�fcb��*/
int drop_fcb(int fcb_id, int r_mode)
{
	int child;
	int origin_fcb_id = fcb_list[fcb_id].filep[0];
	int origin_child;
	int drop;
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		cout << "Drop FCB failed." << endl;
		return -2;
	}
	if (fcb_list[fcb_id].filep[2]!=-1)
	{
		if (r_mode == TRUE)
		{
			for (int i = 2; fcb_list[fcb_id].filep[i] != -1; i++)
			{
				drop_fcb(fcb_list[fcb_id].filep[i], TRUE);
			}
		}
		else
		{
			cout << "FCB not empty. Drop fcb failed!" << endl;
			return -1;
		}
	}

	/*FCB�ÿձ�ʶ*/
	fcb_list[fcb_id].file_type = EMPTY_T;
	fcb_list[fcb_id].filep[0] = -1;
	fcb_list[sys.last_freefcb_id].filep[0] = fcb_id;
	/*д����*/
	update_fcb(sys.last_freefcb_id);
	update_fcb(fcb_id);
	sys.last_freefcb_id = fcb_id;
	update_sys();
	/*ɾ��ԭ������*/
	for (origin_child = 2; fcb_list[origin_fcb_id].filep[origin_child] != -1; origin_child++)
	{
		if (origin_child == EXT_CB)
		{
			origin_fcb_id = fcb_list[origin_fcb_id].filep[origin_child];
			origin_child = 1;
		}
		if (fcb_list[origin_fcb_id].filep[origin_child] == fcb_id)
		{
			//��ǰ�ƶ�filep
			for (drop = origin_child; fcb_list[origin_fcb_id].filep[drop] != -1; drop++)
			{
				if (drop == EXT_CB)
				{
					fcb_list[fcb_list[origin_fcb_id].filep[0]].filep[drop - 1] == fcb_list[origin_fcb_id].filep[1];
					/*д��FCBͷ��*/
					update_fcb(origin_fcb_id);
					origin_fcb_id = fcb_list[origin_fcb_id].filep[drop];
					drop = 1;
				}
				fcb_list[origin_fcb_id].filep[drop] = fcb_list[origin_fcb_id].filep[drop + 1];
				if (fcb_list[origin_fcb_id].filep[drop] == -1)
				{
					break;
				}
			}
			//��extCB
			if (drop == 1)
			{
				//�Ͽ�extCB����
				fcb_list[fcb_list[origin_fcb_id].filep[0]].filep[EXT_CB] = -1;
				update_fcb(fcb_list[origin_fcb_id].filep[0]);
				//�ͷ�extCB
				fcb_list[origin_fcb_id].filep[0] = sys.freefcb_id;
				update_fcb(origin_fcb_id);
				sys.freefcb_id = origin_fcb_id;
				update_sys();
			}
			else
			{
				/*д��FCB*/
				update_fcb(origin_fcb_id);
			}
			break;
		}
	}
	return 0;
}

/*����FCB*/
void update_fcb(int update_fcb_id)
{
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Update FCB failed.\n");
		return;
	}
	fcb_list[update_fcb_id].edit_time = current_time();
	fseek(fp, FCB_POS(update_fcb_id), SEEK_SET);
	fwrite(&fcb_list[update_fcb_id], sizeof(FCB), 1, fp);
	fflush(fp);
	return;
}

int write_ib(int ib_id, int ib_size, FILE* fp_tmp)
{
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Update system failed.\n");
		return 0;
	}
	sys.last_write_time = current_time();
	fseek(fp, IB_POS(ib_id), SEEK_SET);
	fwrite(&sys, sizeof(superBlock), 1, fp);
	fflush(fp);
	return 0;
}
int move_ib(int ib_id, FILE* fp_tmp)
{
	return 0;
}
int erase_ib(int ib_id)
{
	return 0;
}

/*����ϵͳ��*/
void update_sys()
{
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Update system failed.\n");
		return;
	}
	sys.last_write_time = current_time();
	fseek(fp, SUPER_Loacation * BLOCK_SIZE, SEEK_SET);
	fwrite(&sys, sizeof(superBlock), 1, fp);
	fflush(fp);
	return;
}
