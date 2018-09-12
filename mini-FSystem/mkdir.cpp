#include "pch.h"
#include "Fcommand.h"

/***********************************************************************************/
/********************************  �½��ļ��к��� **********************************/
/***********************************************************************************/
int mkdir(char* path, int mk_type)
{
	/**** 1��ָ��Ŀ¼���½�һ��Ŀ¼��ж��Ƿ��Ѵ��ڲ���ʾ ********************/
	/**** 2���½���ͬʱ�����ļ����ƿ飬������Ŀ¼�ĸ���ʱ����ôŪ****/

		/**************** ���������·�� **************/
	char name[13] = { 0 };
	int tmp_fcb_id;
	int i;
	for (i = strlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '\\')
		{
			if (i == strlen(path) - 1)
			{
				cout << "Path ending incorrect. Please re-enter" << endl;
			}
			if (i != 0)
				path[i] = '\0';
			break;
		}
	}
	if (i < 0) {
		strcpy(name, path);
		path[0] = '\0';
		tmp_fcb_id = current_fcb_id;
	}
	else
	{
		strcpy(name, path + i + 1);
		tmp_fcb_id = path_to_fcb_id(path, DIR_T);
	}
	switch (tmp_fcb_id)
	{
	case -2:
	case -1:
		break;
	default:
		new_fcb(tmp_fcb_id, mk_type, name, NULL);
		break;
	}
	return 0;
}