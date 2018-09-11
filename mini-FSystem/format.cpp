#include "pch.h"
#include "Fcommand.h"
/*Format 1G space*/
int format() {
	superBlock block;
	if (strcmp(mini_path, "D:\\") == 0)
	{
		printf("Please create a mini_FS first!\n");
		return -1;
	}
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Format failed.\n");
		return -1;
	}
	rewind(fp);
	fseek(fp, SUPER_Loacation, SEEK_SET);
	//ϵͳ���ʼ��
	block.fcb_Location = FCB_Location;
	block.ib_location = IB_Location;
	block.fcb_size = FCB_SIZE;
	block.blk_size = BLOCK_SIZE;
	block.superblock_size = sizeof(superBlock);
	block.superblock_location = 0;
	block.last_ib = -1;
	block.last_fcb = 0;
	block.freefcb_id = 1;
	block.rootfcb_id = 0;
	block.freeib_id = 0;
	block.last_write_time = current_time();
	fwrite(&block, sizeof(superBlock), 1, fp);

	//���ļ�ͷ��ʼ��
	IB ib;
	fseek(fp, IB_Location * BLOCK_SIZE, SEEK_SET);
	ib.block_num = 4033;
	ib.length = IB_NUM;
	ib.last_num = -1;		//-1��ʾ��Ϊ���ļ���Ϣ��ͷ
	ib.next_num = -1;		//-1��ʾ��Ϊ���һ�ο��ļ���Ϣ��
	fwrite(&ib, sizeof(IB), 1, fp);

	//���ƿ��ʼ��
	FCB fcb;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	strcpy(fcb.filename, "");			//��Ŀ¼�ļ���
	fcb.create_time = current_time();	//����ʱ��
	fcb.edit_time = fcb.create_time;	//�༭ʱ��
	fcb.file_type = EMPTY_T;			//�հ�
	fcb.file_size = 0;					//��СΪ0
	fcb.is_delete = 1;					//��ɾ��
	fcb.file_block_id = -1;				//�ļ���
	for (int i = 0; i < FCB_WIDTH; i++)
		fcb.filep[i] = -1;				//��ʱ��������Ŀ¼
	for (int i = 0; i < FCB_NUM; i++)
	{
		fcb.filep[0] = i + 1;
		fwrite(&fcb, sizeof(FCB), 1, fp);
	}

	//��Ŀ¼��ʼ��
	FCB root;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	strcpy(root.filename, "\\");			//��Ŀ¼�ļ���
	root.create_time = current_time();		//����ʱ��
	root.edit_time = root.create_time;		//�༭ʱ��
	root.file_type = DIR_T;					//�ļ���
	root.file_size = 0;						//���ļ��У���СΪ0
	root.is_delete = 0;						//δɾ��
	root.file_block_id = -1;				//�ļ���
	for (int i = 0; i < FCB_WIDTH; i++)
		root.filep[i] = -1;					//��ʱ��������Ŀ¼
	root.filep[1] = 0;
	fwrite(&root, sizeof(FCB), 1, fp);
	printf("\nFormat success.\n");
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Mount failed.\n");
		return -1;
	}
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
	fseek(fp, IB_Location * BLOCK_SIZE + sys.freeib_id * BLOCK_SIZE, 0);
	fread(&ib_tmp, sizeof(IB), 1, fp);
	current_fcb_id = sys.rootfcb_id;
	/*
	FCB rootd;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	fread(&rootd, sizeof(FCB), 1, fp);
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	for (int i = 0; i < FCB_NUM; i++)
	{
		fread(&fcb_list[i], sizeof(FCB), 1, fp);
	}
	*/
	fclose(fp);
	return 0;
}