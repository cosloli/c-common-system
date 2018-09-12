#include "pch.h"
#include "Fcommand.h"
/*Mount used Mini_FS*/
int mount(char arr[])
{
	mini_path[3] = '\0';
	strcat(mini_path, arr);
	strcat(mini_path, ".dat");
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Mount failed.\n");
		return -1;
	}
	//װ��ϵͳ��
	fseek(fp, SUPER_Loacation * BLOCK_SIZE, SEEK_SET);
	fread(&sys, sizeof(superBlock), 1, fp);
	//װ�ؿ��Ƶ�Ԫ
	//rewind(fp);
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	//cout << ftell(fp) << endl;
	for (int i = 0; i < FCB_NUM; i++)
	{
		fread(&fcb_list[i], sizeof(FCB), 1, fp);
	}
	//װ�ؿ�����Ϣ��ͷ
	fseek(fp, IB_Location * BLOCK_SIZE + sys.freeib_id * BLOCK_SIZE, 0);
	fread(&ib_tmp, sizeof(IB), 1, fp);
	current_fcb_id = sys.rootfcb_id;
	printf("Mount success.\n");
	sys_mounted = 1;
	return 0;
}