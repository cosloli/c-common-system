#include "pch.h"
#include "Fcommand.h"
/*Format 1G space*/
int format() {
	superBlock lump;
	if (strcmp(fs_path, "D:\\") == 0)
	{
		printf("Please create a mini_FS first!\n");
		return -1;
	}
	if ((fp = fopen(fs_path, "rb+")) == NULL)
	{
		printf("Format failed.\n");
		return -1;
	}
	rewind(fp);
	fseek(fp, SUPER_Loacation, SEEK_SET);
	//ϵͳ���ʼ��
	lump.fcb_Location = FCB_Location;
	lump.ib_location = IB_Location;
	lump.fcb_size = FCB_SIZE;
	lump.blk_size = BLOCK_SIZE;
	lump.superblock_size = sizeof(superBlock);
	lump.superblock_location = 0;
	lump.last_write_ib = -1;
	lump.last_write_fcb = 0;
	lump.rootfcb_id = 0;
	lump.freefcb_id = 1;
	lump.freeib_id = 1;
	lump.last_freefcb_id = FCB_NUM - 1;
	lump.last_freeib_id = 1;
	lump.last_write_time = current_time();
	fwrite(&lump, sizeof(superBlock), 1, fp);
	fflush(fp);

	//���ļ�ͷ��ʼ��
	IB_Disk ib;
	for (int i = 0; i < IB_NUM - 1; i++)
	{
		fseek(fp, IB_POS(i), SEEK_SET);
		ib.block_id = 0;
		ib.size = 0;
		ib.last_id = 0;
		ib.next_id = 0;
		fwrite(&ib, sizeof(FCB), 1, fp);
	}
	fseek(fp, IB_Location * BLOCK_SIZE, SEEK_SET);
	ib.block_id = 1;
	ib.size = IB_NUM;
	ib.last_id = 0;		//0��ʾ��Ϊ���ļ���Ϣ��ͷ
	ib.next_id = 0;		//0��ʾ��Ϊ���һ�ο��ļ���Ϣ��
	fwrite(&ib, sizeof(IB_Disk), 1, fp);
	fflush(fp);

	//���ƿ��ʼ��
	FCB fcb;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	strcpy(fcb.filename, "");			//��Ŀ¼�ļ���
	fcb.create_time = current_time();	//����ʱ��
	fcb.edit_time = fcb.create_time;	//�༭ʱ��
	fcb.file_type = EMPTY_T;			//�հ�
	fcb.file_size = 0;					//��СΪ0
	fcb.file_block_id = -1;				//�ļ���
	for (int i = 0; i < FCB_WIDTH; i++)
		fcb.filep[i] = -1;				//��ʱ��������Ŀ¼
	for (int i = 0; i < FCB_NUM - 1; i++)
	{
		fcb.filep[0] = i + 1;
		fwrite(&fcb, sizeof(FCB), 1, fp);
	}
	//��ʼ��ĩλ����FCB
	fcb.filep[0] = -1;
	fwrite(&fcb, sizeof(FCB), 1, fp);
	fflush(fp);
	//��Ŀ¼��ʼ��
	FCB root;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	strcpy(root.filename, "/");			//��Ŀ¼�ļ���
	root.create_time = current_time();		//����ʱ��
	root.edit_time = root.create_time;		//�༭ʱ��
	root.file_type = DIR_T;					//�ļ���
	root.file_size = 0;						//���ļ��У���СΪ0
	root.file_block_id = -1;				//�ļ���
	for (int i = 0; i < FCB_WIDTH; i++)
		root.filep[i] = -1;					//��ʱ��������Ŀ¼
	root.filep[1] = 0;
	fwrite(&root, sizeof(FCB), 1, fp);
	fflush(fp);
	printf("\nFormat success.\n");
	if (fp == NULL)
	{
		printf("Mount failed.\n");
		return -1;
	}
	if (sys_mounted == 1)
	{
		//װ��ϵͳ��
		fseek(fp, SUPER_Loacation * BLOCK_SIZE, SEEK_SET);
		fread(&sys, sizeof(superBlock), 1, fp);
		//װ�ؿ��Ƶ�Ԫ
		fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
		for (int i = 0; i < FCB_NUM; i++)
		{
			fread(&fcb_list[i], sizeof(FCB), 1, fp);
		}
		//װ�ؿ�����Ϣ��ͷ
		fseek(fp, IB_POS(sys.freeib_id), 0);
		fread(&free_ib_tmp, sizeof(IB_Disk), 1, fp);
		free_ib_tree.Clear();
		id_tree.Clear();
		free_ib_tree.Insert(&free_ib_tmp);
		//�������п�ƽ�������
		for (int i = 0; free_ib_tmp.next_id != 0; i++)
		{
			fseek(fp, IB_POS(free_ib_tmp.next_id), 0);
			fread(&free_ib_tmp, sizeof(IB_Disk), 1, fp);
			free_ib_tree.Insert(&free_ib_tmp);
		}
		current_fcb_id = sys.rootfcb_id;
		strcpy(current_path, "/");
	}
	fflush(fp);
	return 0;
}