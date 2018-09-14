#pragma once
#ifndef F_BASE_H
#define F_BASE_H
#include "pch.h"
#include <iostream>
#include <windows.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stack>

using namespace std;

/*������ṹ*/
typedef struct Super_block {
	int fcb_Location;			//��һ���ļ����ƿ��λ��
	int ib_location;			//��һ�����ݿ��λ��
	int fcb_size;				//fcb��С
	int blk_size;				//���С
	int superblock_size;		//�������С
	int superblock_location;	//������λ��
	int last_write_ib;			//���д�����Ϣ��
	int last_write_fcb;			//���д����ļ����ƿ�
	int freefcb_id;				//��һ�����п��ƿ�λ�� (0,258047)
	int freeib_id;				//��һ��������Ϣ��λ�ã�4033��262144��
	int last_freefcb_id;		//���һ�����п��ƿ�λ��
	int last_freeib_id;			//���һ��������Ϣ��λ��
	int rootfcb_id;				//��Ŀ¼λ��
	int last_write_time;		//���һ��д��ʱ��
}superBlock;

/*���Ƶ�Ԫ�ṹ*/
typedef struct F_CB {
	int create_time;		//�ļ�����ʱ��
	int edit_time;			//�ļ�����ʱ��
	int file_size;			//�ļ�ռ���ֽ���
	int file_block_id;		//�ļ�ͷ����
	int filep[FCB_WIDTH];	//��Ŀ¼�������
	char filename[14];		//�ļ�����
	char file_type;			//�ļ�����
}FCB;

/*��չ���Ƶ�Ԫ�ṹ*/
typedef struct ext_CB {
	int filep[16];			//��Ŀ¼���
}extCB;

/*�ļ���Ԫ�ṹ*/
typedef struct I_B {
	int block_id;			//�Լ��Ŀ��
	int size;				//��������
	int last_id;			//ǰ���ͷ�Ŀ��
	int next_id;			//�����ͷ�Ŀ��
}IB_Disk;

class IB_AVLNode {
	friend class IB_AVLTree;
private:
	int IB_id_;
	int size_;
	int last_id_;
	int next_id_;
	IB_AVLNode* p_lchild_;
	IB_AVLNode* p_rchild_;
	IB_AVLNode* p_parent_;
	int height_;
public:
	IB_AVLNode();  //Ĭ�Ϲ��캯��������;
	IB_AVLNode(const int id);	//��IDΪ����ֵ
	IB_AVLNode(const int id, const int size, const int last_id, const int next_id);  //��sizeΪ����ֵ�������г�Ա��������ֵ;
	IB_AVLNode(const IB_AVLNode & ib);
	IB_AVLNode(const IB_Disk & ib);
	void SetValue(const int id, const int size);
	void SetValue(const IB_Disk & ib);
	void Print();
	int GetSize() const;
	int GetId() const;
	int Last() const;
	int Next() const;
	IB_AVLNode* lchild();
	IB_AVLNode* rchild();
	IB_AVLNode* parent();
};


class IB_AVLTree {
private:
	IB_AVLNode *root_;

public:
	IB_AVLTree();    //��root����ֵ;

	~IB_AVLTree();   //����Clear()���������ͷ��ڴ�;

	void Insert(const int id);		//����ID
	void Insert(const int id, const int size, const int last_id, const int next_id);      //����һ�����;
	void Insert(IB_Disk* ib);							//����һ��free ib;
	void Insert(IB_AVLNode* ib);

	void Delete(const int id);		//ɾ��ID
	void Delete(const int id, const int size);      //ɾ��IB_idΪid�Ľ�㣬��û����ɾ��;
	void Delete(IB_Disk* ib);
	void Delete(IB_AVLNode* ib);

	IB_AVLNode* GetRoot();
	IB_AVLNode* Search(const int id, const int size);  //����ֵΪsize�Ľ��;
	IB_AVLNode* Search(IB_AVLNode* ib);
	IB_Disk* Search(IB_Disk* ib);

	void PrintDLR();     //�ȸ�������ӡ������;

	void PrintLDR();     //�и�������ӡ��; �˺�����ӡ����������ݣ��ɹ�����;

	void PrintRDL();     //���������ӡ��

	void Clear();        //������գ�ɾ�����н��;

private:
	//��������жϲ��������־������ת�㷨����һ��;
	void Rotate(IB_AVLNode *p_node);

	//������תһ��,������ʧȥƽ������;
	//����������heightֵ;
	void LL(IB_AVLNode *p_node);

	//������תһ��,������ʧȥƽ������;
	//����������heightֵ;
	void RR(IB_AVLNode *p_node);

	//�������������ת����;
	//p_parent���²�����ĸ����,�˽��heightֵδ����;
	void InsertAdjust(IB_AVLNode* p_parent_new_node);

	//ɾ���������ĵ���;
	//p_parent_deleted_node�Ǳ�ɾ�����ĸ����;�˽��heightֵδ����;
	void DeleteAdjust(IB_AVLNode* p_parent_deleted_node);

	//���㲢����p_node��ָ�����balanceֵ;
	int GetBalance(IB_AVLNode* p_node);

	//����p_nodeָ�����heightֵ;
	void UpdateHeight(IB_AVLNode* p_node);

	//ɾ�����ʱ�ҵ���ɾ����������ǰ�����̣����������ɾ�����;
	IB_AVLNode* FindBestReplacement(IB_AVLNode* p_node);

	//�ҵ���С��ƽ������������������,p_node��heightֵ����Ϊδ���µ�;
	IB_AVLNode* FindMinUnbalanceSubtree(IB_AVLNode *p_node);

	//�����ӡ���ֵ;
	void PrintLDR(IB_AVLNode* p_node);

	//�����ӡ���ֵ;
	void PrintRDL(IB_AVLNode* p_node);     //���������ӡ��
};

/*ȫ�ֱ���*/
extern FILE *fp;
extern superBlock sys;
extern FCB fcb_list[FCB_NUM];
extern IB_AVLTree free_ib_tree;
extern IB_AVLTree id_tree;
extern IB_Disk free_ib_tmp;
extern int current_fcb_id;
extern int sys_mounted;
extern char* ib_tmp;
extern char fs_path[100];
extern char current_path[128];
/*ȫ�ֱ���*/

#endif // !F_BASE_H


