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
typedef struct SentNode{           //子句节点类型
	ElemType data;                 //数据子句长度
	struct BoolNode *FirstBool;    //子句第一个变元节点指针
	struct SentNode *nextsent;     //下一个子句节点指针
}SentNode;

typedef struct BoolNode{           //布尔节点类型
    ElemType data;                 //变元的值
    struct BoolNode *nextbool;     //下一个变元节点指针
}BoolNode;
typedef struct{                    //cnf文件结构体
    SentNode *FirstSent;           //第一个子句
    int BoolElem;                  //布尔变元数
    int Sent;                      //子句数
}ALGraph;

status ReadDnfFile(FILE *fp,ALGraph *G);          //读取cnf文件
status InitSent(SentNode *S);                     //初始化cnf结构体
status SentInsert(SentNode *S);                   //插入子句结构
status BoolInsert(BoolNode *B,ElemType e);        //插入变元节点
status TarverseBool(ALGraph *G);                  //遍历cnf文件
status DestroyBool(ALGraph *G);                   //销毁cnf结构体
status CopyName(char *filename,char *resultname); //复制文件名
#endif // CNFFILE
