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
	int last_free_ib;			//ǰ���ͷ�Ŀ��
	int next_free_ib;			//�����ͷ�Ŀ��
}IB_Disk;

class IB_AVLNode {
	friend class IB_AVLTree;
private:
	int size_;
	IB_AVLNode* p_lchild_;
	IB_AVLNode* p_rchild_;
	IB_AVLNode* p_parent_;
	int height_;
	int IB_id;
public:
	IB_AVLNode();  //Ĭ�Ϲ��캯�����Գ�size_, IB_id������г�Ա��������ֵ;
	IB_AVLNode(const int id, const int key);  //�����г�Ա��������ֵ;
	void SetValue(const int id, const int key);
	void Print();
	int GetSize() const;
	int GetId() const;
};


class IB_AVLTree {
private:
	IB_AVLNode *root_;

public:
	IB_AVLTree();    //��root����ֵ;

	~IB_AVLTree();   //����Clear()���������ͷ��ڴ�;

	void Insert(const int id, const int size);      //����һ�����;
	void Insert(IB_Disk ib);							//����һ��free ib;

	void Delete(const int id, const int size);      //ɾ��IB_idΪid�Ľ�㣬��û����ɾ��;
	void Delete(IB_Disk ib);

	IB_AVLNode* Search(const int id, const int size);  //����ֵΪsize�Ľ��;
	IB_AVLNode* Search(IB_Disk ib);

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
extern IB_Disk free_ib_tmp;
extern int current_fcb_id;
extern int sys_mounted;
extern char* ib_tmp;
extern char fs_path[100];
extern char current_path[128];
/*ȫ�ֱ���*/

#endif // !F_BASE_H


