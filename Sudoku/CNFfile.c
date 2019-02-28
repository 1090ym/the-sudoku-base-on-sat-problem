#include "CNFfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//初始化子句集函数
status InitSent(SentNode *S){
	if(S!=NULL){
        S->data = 1;
        S->FirstBool = (BoolNode*)malloc(sizeof(BoolNode));
        S->FirstBool->nextbool = NULL;
        S->nextsent = NULL;
        return OK;
	}
    return ERROR;
}

//插入单子句函数
status SentInsert(SentNode *S){
    SentNode *p,*q;
    int j = 2;
    p = S;
    while(p->nextsent){
        p = p->nextsent;
        j++;
    }
    if(!p)  return OVERFLOW;
    q = (SentNode*)malloc(sizeof(SentNode));
    q->data = j;
    q->nextsent = p->nextsent;
    p->nextsent = q;
    return TRUE;
}

//插入布尔变元函数
status BoolInsert(BoolNode *B,ElemType e)
{
    BoolNode *p,*q;
    p = B;
    while(p->nextbool){
        p = p->nextbool;
    }
    if(!p)  return OVERFLOW;
    q = (BoolNode*)malloc(sizeof(BoolNode));
    q->data = e;
    q->nextbool = p->nextbool;
    p->nextbool = q;
    return TRUE;
}

//读取cnf文件函数
status ReadDnfFile(FILE *fp,ALGraph *G)
{
    SentNode *s;
    ALGraph *R;
    R = G;
    s = R->FirstSent;
    int Bool=1;
    int i = 0;
    char ch;
    fscanf(fp,"%c",&ch);
    while(ch == 'c')
    {
        while(ch != '\n')
        {
            fscanf(fp,"%c",&ch);
        }
        fscanf(fp,"%c",&ch);
    }
    if(ch == 'p')
    {
        for(i = 0;i < 5;i++)
            fscanf(fp,"%c",&ch);
        fscanf(fp,"%d",&G->BoolElem);
        fscanf(fp,"%c",&ch);
        fscanf(fp,"%d",&G->Sent);
    }
    InitSent(R->FirstSent);
    for(i = 0;i < R->Sent;i++)
    {
        s->data = 0;
        fscanf(fp,"%d",&Bool);
        while(Bool != 0)
        {
            BoolInsert(s->FirstBool,Bool);
            s->data++;
            fscanf(fp,"%d",&Bool);
        }
        if(i == R->Sent-2)
        {
            s->nextsent = NULL;
            break;
        }
        if(Bool == 0){
            SentInsert(R->FirstSent);
            s = s->nextsent;
            s->FirstBool = (BoolNode*)malloc(sizeof(BoolNode));
            s->FirstBool->nextbool = NULL;
        }
    }
    return TRUE;
}

//遍历cnf文件函数
status TarverseBool(ALGraph *G)
{
    ALGraph *R;
    R = G;
    SentNode *s;
    s = R->FirstSent;
    if(s == NULL){
        printf("空集\n");
        return TRUE;
    }
    BoolNode *b;
    b = s->FirstBool->nextbool;
    if(s->FirstBool == NULL)
    {
        return TRUE;
    }
    while(s)
    {

        while(b)
        {
            printf("%d  ",b->data);
            b = b->nextbool;
        }
        printf("\n");
        if(s->nextsent==NULL)
            break;
        s = s->nextsent;
        b = s->FirstBool->nextbool;
    }
    return TRUE;
}

//销毁cnf结构
status DestroyBool(ALGraph *G)
{
    ALGraph *R;
    R = G;
    SentNode *s;
    s = R->FirstSent;
    BoolNode *b,*p;
    b = s->FirstBool->nextbool;

    while(s)
    {
        while(b)
        {
            p = b;
            b = b->nextbool;
            free(p);
        }
        s->FirstBool = NULL;
        if(s->nextsent==NULL)
            break;
        s = s->nextsent;
        b = s->FirstBool->nextbool;
    }
    G->BoolElem = 0;
    G->Sent = 0;
    return TRUE;
}

//复制文件名
status CopyName(char *filename,char *resultname)
{
    int i = 0;
    while(filename[i] != '.')
    {
        resultname[i] = filename[i];
        i++;
    }
    resultname[i] = '\0';
    return TRUE;
}
