#include "pch.h"
#include "Fcore.h"

/***************************************��ȡʱ�亯��*********************************/
/************************************************************************************/
int current_time()
{
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

/***************************************��ӡʱ�亯��*********************************/
/************************************************************************************/
void print_time(int timestamp)
{
	int ntime;
	char sTime[20] = { 0 };						//�ú����������<time.h><string.h>ͷ�ļ�
	time_t ftime = timestamp + (8 * 3600);
	ntime = (int)ftime;
	tm* pTime = gmtime(&ftime);
	strftime(sTime, 80, "%Y-%m-%d %H:%M:%S", pTime);//����1
	cout << sTime << endl;
	return;
}
/************************************************************************************/


/***************************************��ӡʱ�亯��*********************************/
/************************************************************************************/
void print_time(int timestamp)
{		                                         //nowtime:��ǰϵͳʱ��,last_write_time[15]:���д��ʱ�䣬�������յ�ǰϵͳʱ��
	int ntime;
	char sTime[20] = { 0 };						//�ú����������<time.h><string.h>ͷ�ļ�
	time_t ftime = timestamp + (8 * 3600);
	ntime = (int)ftime;
	tm* pTime = gmtime(&ftime);
	strftime(sTime, 80, "%Y-%m-%d %H:%M:%S", pTime);//����1
	cout << sTime;
	return;
}
/************************************************************************************/
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
		if (str[i] == '/')
		{
			if (i && str[i - 1] == '/')
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
				fcb_id = fcb_list[fcb_id].filep[EXT_CB];
				j = 1;
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

/*����fcb�����IB*/
int new_fcb(int dir_fcb_id, int fcb_type, char *name, char *src_f_path)
{
	int free_id;
	int child;
	int fcb_id_tmp = dir_fcb_id;
	int file_size;
	IB_Disk* file_info;
	IB_AVLNode* p_free_ib;
	FILE* fp_tmp;
	if (fp == NULL)
	{
		cout << "New fcb failed." << endl;
		return -2;
	}
	/*������*/
	for (child = 2; fcb_list[fcb_id_tmp].filep[child] != -1; child++)
	{
		if (child == EXT_CB)
		{
			fcb_id_tmp = fcb_list[fcb_id_tmp].filep[EXT_CB];
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
	/*��ʼ���հ�FCB*/
	fcb_list[free_id].file_type = DIR_T;
	fcb_list[free_id].create_time = current_time();
	fcb_list[free_id].filep[0] = fcb_id_tmp;
	fcb_list[free_id].filep[1] = free_id;
	/*дӲ��*/
	write_fcb(free_id);
	write_fcb(fcb_id_tmp);
	write_sys();
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
		fcb_list[free_id].filep[0] = fcb_id_tmp;
		fcb_list[free_id].filep[1] = free_id;
		strcpy(fcb_list[free_id].filename, name);
		/*дӲ��*/
		write_fcb(free_id);
		write_fcb(fcb_id_tmp);
		write_sys();
		/*ת���հ�FCB*/
		fcb_id_tmp = free_id;
	}
	else
	{
		strcpy(fcb_list[fcb_id_tmp].filename, name);
		write_fcb(fcb_id_tmp);
	}
	//����IB
	if (fcb_type == FILE_T)
	{
		fcb_list[fcb_id_tmp].file_type = FILE_T;
		if (src_f_path)
		{
			if (src_f_path)
			{

			}
			while (true)
			{
				/*���IB�ռ�*/
				p_free_ib = get_free_ib(1);
				if (p_free_ib == NULL)
				{
					cout << "Disk full. Delete something to create." << endl;
					return -3;
				}
				else
				{
					break;
				}
			}
		}
		
		file_info = write_ib(BLOCK_SIZE, NULL);
		if (file_info != NULL)
		{
			fcb_list[fcb_id_tmp].file_block_id = file_info->block_id;
			fcb_list[fcb_id_tmp].file_size = 1 * BLOCK_SIZE - sizeof(IB_Disk);
			sys.last_write_ib = file_info->block_id;
			write_sys();
		}
		write_fcb(fcb_id_tmp);
	}
	fflush(fp);
	return 0;
}

/*�ƶ�FCB*/
int move_fcb(int dir_fcb_id, int fcb_id, char *name, char* file_path)
{
	int child;
	int free_fcb_id;
	int tmp_fcb_id = dir_fcb_id;
	int origin_fcb_id = fcb_list[fcb_id].filep[0];
	int origin_child;
	int same_origin = 0;;
	int drop;
	if (fp == NULL)
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
			tmp_fcb_id = fcb_list[tmp_fcb_id].filep[EXT_CB];
			child = 1;
		}
		if (strcmp(name, fcb_list[fcb_list[tmp_fcb_id].filep[child]].filename) == 0)
		{
			cout << "Name duplicated. Please re-enter." << endl;
			return -1;
		}
		if (fcb_id == fcb_list[tmp_fcb_id].filep[child])
		{
			same_origin = 1;
		}
	}
	if (child == EXT_CB)
	{
		//����հ�FCB
		free_fcb_id = sys.freefcb_id;
		fcb_list[tmp_fcb_id].filep[EXT_CB] = free_fcb_id;//����extCB
		sys.freefcb_id = fcb_list[free_fcb_id].filep[0];
		sys.last_write_fcb = free_fcb_id;
		//��ʼ���հ�FCB
		fcb_list[free_fcb_id].file_type = EXT_T;
		fcb_list[free_fcb_id].create_time = current_time();
		fcb_list[free_fcb_id].edit_time = fcb_list[free_fcb_id].create_time;
		fcb_list[free_fcb_id].filep[0] = tmp_fcb_id;
		fcb_list[free_fcb_id].filep[1] = free_fcb_id;
		/*дӲ��*/
		write_fcb(free_fcb_id);
		write_fcb(tmp_fcb_id);
		write_sys();
		/*ת���հ���չFCB*/
		tmp_fcb_id = free_fcb_id;
		child = 1;
	}
	/*���·��*/
	//fcb_list[fcb_id].filep[0] = 0;
	if (!same_origin)
	{
		/*������·��*/
		fcb_list[tmp_fcb_id].filep[child] = fcb_id;
		fcb_list[fcb_id].filep[0] = dir_fcb_id;
		write_fcb(tmp_fcb_id);
		write_fcb(fcb_id);
		/*���ݸ�Ŀ¼�ڵ�*/
		while (fcb_list[origin_fcb_id].file_type == EXT_CB)
		{
			origin_fcb_id = fcb_list[origin_fcb_id].filep[0];
		}
		/*ɾ��ԭ����*/
		for (origin_child = 2; fcb_list[origin_fcb_id].filep[origin_child] != -1; origin_child++)
		{
			if (origin_child == EXT_CB)
			{
				origin_fcb_id = fcb_list[origin_fcb_id].filep[EXT_CB];
				origin_child = 1;
			}
			if (fcb_list[origin_fcb_id].filep[origin_child] == fcb_id)
			{
				//��ǰ�ƶ�filep
				for (drop = origin_child; fcb_list[origin_fcb_id].filep[drop] != -1; drop++)
				{
					if (drop == EXT_CB)
					{
						fcb_list[origin_fcb_id].filep[EXT_CB - 1] = fcb_list[fcb_list[origin_fcb_id].filep[EXT_CB]].filep[1];
						write_fcb(origin_fcb_id);
						origin_fcb_id = fcb_list[origin_fcb_id].filep[EXT_CB];
						drop = 1;
						/*���ļ����ڵ��ض���*/
						fcb_list[fcb_list[origin_fcb_id].filep[1]].filep[0] = fcb_list[origin_fcb_id].filep[0];
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
					write_fcb(fcb_list[origin_fcb_id].filep[0]);
					//�ͷ�extCB
					fcb_list[origin_fcb_id].filep[0] = sys.freefcb_id;
					write_fcb(origin_fcb_id);
					sys.freefcb_id = origin_fcb_id;
					write_sys();
				}
				else
				{
					write_fcb(origin_fcb_id);
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
	write_fcb(fcb_id);
	return 0;
}

/*�ͷ�fcb��*/
int drop_fcb(int fcb_id, int r_mode)
{
	int child;
	int origin_fcb_id = fcb_list[fcb_id].filep[0];
	int origin_child;
	int drop;
	int next_fcb_id;
	if (fp == NULL)
	{
		cout << "Drop FCB failed." << endl;
		return -2;
	}
	if (fcb_list[fcb_id].filep[2]!=-1)
	{
		if (r_mode == TRUE)
		{
			next_fcb_id = fcb_id;
			for (int i = 2; fcb_list[next_fcb_id].filep[i] != -1; i++)
			{
				if (i == EXT_CB)
				{
					next_fcb_id = fcb_list[next_fcb_id].filep[EXT_CB];
				}
				drop_fcb(fcb_list[next_fcb_id].filep[i], TRUE);
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
	write_fcb(sys.last_freefcb_id);
	write_fcb(fcb_id);
	sys.last_freefcb_id = fcb_id;
	write_sys();
	/*���ݸ�Ŀ¼�ڵ�*/
	while (fcb_list[origin_fcb_id].file_type == EXT_CB)
	{
		origin_fcb_id = fcb_list[origin_fcb_id].filep[0];
	}
	/*ɾ��ԭ����*/
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
					fcb_list[origin_fcb_id].filep[EXT_CB - 1] = fcb_list[fcb_list[origin_fcb_id].filep[EXT_CB]].filep[1];
					/*д��FCBͷ��*/
					write_fcb(origin_fcb_id);
					origin_fcb_id = fcb_list[origin_fcb_id].filep[EXT_CB];
					drop = 1;
					/*���ļ����ڵ��ض���*/
					fcb_list[fcb_list[origin_fcb_id].filep[1]].filep[0] = fcb_list[origin_fcb_id].filep[0];
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
				write_fcb(fcb_list[origin_fcb_id].filep[0]);
				//�ͷ�extCB
				fcb_list[origin_fcb_id].filep[0] = sys.freefcb_id;
				write_fcb(origin_fcb_id);
				sys.freefcb_id = origin_fcb_id;
				write_sys();
			}
			else
			{
				/*д��FCB*/
				write_fcb(origin_fcb_id);
			}
			break;
		}
	}
	return 0;
}

/*����FCB*/
void write_fcb(int update_fcb_id)
{
	if (fp == NULL)
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

/*��ȡ�հ�IB*/
IB_AVLNode* get_free_ib(int ib_size)
{
	IB_AVLNode* p_ib;
	p_ib = free_ib_tree.Search(-1, ib_size);
	return p_ib;
}

/*��ȡIB��Ϣ*/
IB_Disk* get_ib_info(int ib_id)
{
	IB_Disk* p_ib = new IB_Disk;
	if (fp == NULL)
	{
		printf("Read IB failed.\n");
		return 0;
	}
	fseek(fp, IB_POS(ib_id), SEEK_SET);
	fread(p_ib, sizeof(IB_Disk), 1, fp);

	return p_ib;
}

/*��ȡIB����*/
int get_ib_content(int ib_id)
{
	return 0;
}

/*д��IB*/
IB_Disk* write_ib(int f_size, FILE* fp_tmp)
{
	int ib_id;
	char *buffer;
	int blk_size;
	IB_Disk* ib_info = new IB_Disk;
	IB_AVLNode *p_free_ib = new IB_AVLNode;
	IB_AVLNode *p_new_ib;

	if (fp == NULL)
	{
		printf("Write IB failed.\n");
		return NULL;
	}
	/*���IB�ռ�*/
	p_free_ib = get_free_ib(1);
	if (p_free_ib == NULL)
	{
		cout << "Disk full. Delete something to create." << endl;
		return NULL;
	}
	/*����ռ�ÿ���*/
	blk_size = (f_size + sizeof(IB_Disk)) / BLOCK_SIZE;
	if ((f_size + sizeof(IB_Disk)) % BLOCK_SIZE != 0)
	{
		blk_size++;
	}
	ib_id = p_free_ib->GetId();
	ib_info->block_id = ib_id;
	ib_info->size = blk_size;
	fseek(fp, IB_POS(ib_id), SEEK_SET);
	fwrite(ib_info, sizeof(IB_Disk), 1, fp);
	if (fp_tmp == NULL)
	{
		buffer = new char[f_size];
		memset(buffer, 0, f_size * sizeof(char));
		fwrite(buffer, f_size, 1, fp);
	}
	else
	{
		fwrite(fp_tmp, f_size, 1, fp);
	}
	fflush(fp);
	p_new_ib = new IB_AVLNode(ib_id + blk_size, ib_id - blk_size);
	/*����IB�������*/
	update_ib(p_free_ib, p_new_ib);
	return ib_info;
}

/*�ƶ�IB*/
int move_ib(int src_ib_id, int dst_ib_id)
{
	return 0;
}

/*�ͷ�IB*/
int drop_ib(int ib_id)
{

	return 0;
}

/*����IB�������*/
void update_ib(IB_AVLNode* old_ib, IB_AVLNode* new_ib)
{

}

/*����ϵͳ��*/
void write_sys()
{
	if ((fp = fopen(fs_path, "rb+")) == NULL)
	{
		printf("Write system failed.\n");
		return;
	}
	sys.last_write_time = current_time();
	fseek(fp, SUPER_Loacation * BLOCK_SIZE, SEEK_SET);
	fwrite(&sys, sizeof(superBlock), 1, fp);
	fflush(fp);
	return;
}
