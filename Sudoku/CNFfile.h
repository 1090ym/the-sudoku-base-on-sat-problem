#ifndef CNFFILE_H
#define CNFFILE_H
#include <stdio.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

typedef int status;
typedef int ElemType;

#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef struct SentNode{           //�Ӿ�ڵ�����
	ElemType data;                 //�����Ӿ䳤��
	struct BoolNode *FirstBool;    //�Ӿ��һ����Ԫ�ڵ�ָ��
	struct SentNode *nextsent;     //��һ���Ӿ�ڵ�ָ��
}SentNode;

typedef struct BoolNode{           //�����ڵ�����
    ElemType data;                 //��Ԫ��ֵ
    struct BoolNode *nextbool;     //��һ����Ԫ�ڵ�ָ��
}BoolNode;
typedef struct{                    //cnf�ļ��ṹ��
    SentNode *FirstSent;           //��һ���Ӿ�
    int BoolElem;                  //������Ԫ��
    int Sent;                      //�Ӿ���
}ALGraph;

status ReadDnfFile(FILE *fp,ALGraph *G);          //��ȡcnf�ļ�
status InitSent(SentNode *S);                     //��ʼ��cnf�ṹ��
status SentInsert(SentNode *S);                   //�����Ӿ�ṹ
status BoolInsert(BoolNode *B,ElemType e);        //�����Ԫ�ڵ�
status TarverseBool(ALGraph *G);                  //����cnf�ļ�
status DestroyBool(ALGraph *G);                   //����cnf�ṹ��
status CopyName(char *filename,char *resultname); //�����ļ���
#endif // CNFFILE
