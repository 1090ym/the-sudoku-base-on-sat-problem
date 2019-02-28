#include "CNFfile.h"
#include "DPLL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int value[1100];  //ȫ�ֱ����������洢��Ԫ����ֵ

//��ʼ��ȫ�ֱ���
status InitValue(ALGraph *G)
{
    int i;
    for(i = 0;i < 1100;i++)
    {
         value[i] = -2;
    }
    return TRUE;
}

//�ҵ����Ӿ䣬����ȡ�õ��Ӿ�ı�Ԫֵ
status FindUniSent(ALGraph *G)
{
    ALGraph *R;
    R = G;
    SentNode *s;
    s = R->FirstSent;
    if(s == NULL)
    {
        return OVERFLOW;
    }
    BoolNode *b;
    b = s->FirstBool->nextbool;

    while(s)
    {
        //printf("ok2");
        if(b&&(b->nextbool == NULL)){
            return b->data;
        }
        if(s->nextsent == NULL)
            break;
        s = s->nextsent;
        b = s->FirstBool->nextbool;
    }
    return FALSE;
}

//�ж��Ƿ���ڿ��Ӿ�
status FindEmptySent(ALGraph *G)
{
    ALGraph *R;
    R = G;
    SentNode *s;
    s = R->FirstSent;
    if(s == NULL)
    {
        return FALSE;
    }
    BoolNode *b;
    b = s->FirstBool->nextbool;

    while(s)
    {
        if(b == NULL){
            return TRUE;
        }
        if(s->nextsent == NULL)
            break;
        s = s->nextsent;
        b = s->FirstBool->nextbool;
    }
    return FALSE;
}

//����һ�����Ӿ�
status InsertUniSent(ALGraph *G,int boo)
{
    ALGraph *R;
    R = G;
    SentNode *s,*q;
    s = R->FirstSent;
    if(s == NULL)
    {
        return OVERFLOW;
    }
    BoolNode *p;

    q = (SentNode*)malloc(sizeof(SentNode));
    q->FirstBool = (BoolNode*)malloc(sizeof(BoolNode));
    q->FirstBool->nextbool = NULL;
    p = (BoolNode*)malloc(sizeof(BoolNode));
    p->data = boo;
    p->nextbool = NULL;
    q->FirstBool->nextbool = p;
    q->nextsent = s;
    R->FirstSent = q;
    G->Sent++;
    return TRUE;
}

//ɾ���෴�ı�Ԫ
status DeleteNoBool(ALGraph *G,int boo)
{
    ALGraph *R;
    R = G;
    SentNode *s;
    s = R->FirstSent;
    if(s == NULL)
    {
        return OVERFLOW;
    }
    BoolNode *b,*p;
    b = s->FirstBool->nextbool;
    p = s->FirstBool;

    while(s)
    {
        while(b)
        {
           if(b->data == -boo)
           {
               p->nextbool = b->nextbool;
               free(b);
               s->data--;
               break;
           }
           b = b->nextbool;
           p = p->nextbool;
        }
        if(s->nextsent==NULL)
            break;
        s = s->nextsent;
        b = s->FirstBool->nextbool;
        p = s->FirstBool;
    }
    return TRUE;
}

//ʹ�ò���1�����Ż���DPLL�㷨��ֱ��ѡȡ��һ���Ӿ�ĵ�һ����Ԫ
status DPLL(ALGraph *G)
{
    int unisent;
    ALGraph *R,*cop;
    R = G;
    SentNode *s;
    s = R->FirstSent;
    if(s == NULL)
    {
        return TRUE;
    }
    BoolNode *b;
    b = s->FirstBool->nextbool;

    while(FindUniSent(R) != 0)               //���Ӿ���򻯼�
    {
        unisent = FindUniSent(R);
        if(unisent > 0)
            value[abs(unisent)] = 1;
        else
            value[abs(unisent)] = 0;
        DeleteSent(R,unisent);
        DeleteNoBool(R,unisent);
        if(G->FirstSent == NULL)
        {
            return TRUE;
        }
        if(FindEmptySent(R))
        {
            return FALSE;
        }

    }
    s = R->FirstSent;
    while(s)        //���Ѳ���
    {
        if(s->FirstBool->nextbool != NULL)
        {
            b = s->FirstBool->nextbool;
            unisent = b->data;
            break;
        }
        if(s->nextsent==NULL)
            break;
        s = s->nextsent;
    }
    CopyCnf(R,&cop);
    InsertUniSent(R,unisent);
    if(DPLL(R))
    {
        return TRUE;
    }
    else
    {
        InsertUniSent(cop,-unisent);
        return DPLL(cop);
    }
}

//��ӡ����Ԫ����ֵ
status PrintValue(ALGraph *G)
{
     int i = 1;
     while(i != (G->BoolElem+1)){
            printf("%d    %d\t\t",i,value[i]);
            if((i % 6) == 0)
                printf("\n");
            i++;
     }
     if((i-1)%6 != 0)
         printf("\n");
     return TRUE;
}

status CheckResult(ALGraph *G)
{
    int sen=0;
    ALGraph *R;
    R = G;
    SentNode *s;
    s = R->FirstSent;
    if(s == NULL){
        printf("�ռ�\n");
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
        sen  = 0;
        while(b)
        {
            printf("%d  ",b->data);
            if((b->data) > 0)
            {
                if(value[b->data] == 1)
                    sen++;
            }
            if((b->data) < 0)
            {
                if(value[b->data] == 0)
                    sen++;
            }
            b = b->nextbool;
        }
        if(sen > 0)
            printf("1");
        if(sen < 0)
            printf("0");
        printf("\n");
        if(s->nextsent==NULL)
            break;
        s = s->nextsent;
        b = s->FirstBool->nextbool;
    }
    return TRUE;
}

//ɾ�������ñ�Ԫ���Ӿ�
status DeleteSent(ALGraph *G,int boo)
{
    if(G->FirstSent == NULL)
    {
        return OVERFLOW;
    }
    ALGraph *R;
    R = G;
    SentNode *s,*prs,*q;
    prs = NULL;
    InsertUniSent(R,0);
    prs = R->FirstSent;
    q = prs;
    R->FirstSent = prs->nextsent;
    s = R->FirstSent;
    BoolNode *b,*p;
    b = NULL;
    p = NULL;

        while(q->nextsent != NULL)
        {
            if(FindBool(q->nextsent,boo))
            {
                b = q->nextsent->FirstBool->nextbool;
                while(b)
                {
                    p = b;
                    b = b->nextbool;
                    free(p);
                }
                if(q->nextsent == R->FirstSent)
                {
                    s = q->nextsent;
                    if(s->nextsent){
                        q->nextsent = s->nextsent;
                        R->FirstSent  = s->nextsent;
                    }
                    else
                    {
                        q->nextsent = NULL;
                        R->FirstSent = NULL;
                    }
                    G->Sent--;
                    free(s);
                }
                else
                {
                    s = q->nextsent;
                    if(s->nextsent)
                        q->nextsent = s->nextsent;
                    else
                        q->nextsent = NULL;
                    G->Sent--;
                    free(s);
                }
                if(q->nextsent == NULL)
                    break;
            }
            else
            {
                q = q->nextsent;
            }
        }
    return TRUE;
}

//�ж�һ���Ӿ����Ƿ���ڸñ�Ԫ
status FindBool(SentNode *s,int boo)
{
    if(s == NULL)
    {
        return OVERFLOW;
    }
    BoolNode *b;
    b = s->FirstBool->nextbool;
    while(b)
    {
        if(b->data == boo){
            return TRUE;
        }
        b = b->nextbool;
    }
    return FALSE;
}

//����һ��cnf�ṹ
status CopyCnf(ALGraph *G,ALGraph **C)
{
    ALGraph *R;
    R = G;
    (*C) = (ALGraph*)malloc(sizeof(ALGraph));
    (*C)->FirstSent = (SentNode*)malloc(sizeof(SentNode));
    (*C)->BoolElem = R->BoolElem;
    (*C)->Sent = R->Sent;
    InitSent((*C)->FirstSent);

    SentNode *s,*c;
    s = R->FirstSent;
    c = (*C)->FirstSent;
    if(s == NULL){
        (*C)->FirstSent = NULL;
        return OVERFLOW;
    }
    BoolNode *b;
    b = s->FirstBool->nextbool;

    while(s)
    {
        while(b)
        {
            BoolInsert(c->FirstBool,b->data);
            b = b->nextbool;
        }
        if(s->nextsent==NULL)
            break;
        SentInsert((*C)->FirstSent);
        s = s->nextsent;
        b = s->FirstBool->nextbool;
        c = c->nextsent;
        c->FirstBool = (BoolNode*)malloc(sizeof(BoolNode));
        c->FirstBool->nextbool = NULL;
    }
    return TRUE;
}

//��cnf�ļ��������д���ļ�
status WriteCnfValue(FILE *fp,int result,int Bool,double t)
{
    int j = 1;
    fprintf(fp,"%c",'s');
    fprintf(fp,"%c",' ');
    fprintf(fp,"%d",result);
    fprintf(fp,"%c",'\n');
    fprintf(fp,"%c",'v');
    fprintf(fp,"%c",' ');
    while(j != (Bool+1))
    {
        if(value[j] == 1)
        {
            fprintf(fp,"%d",j);
            fprintf(fp,"%c",' ');
        }
        if(value[j] == 0)
        {
            fprintf(fp,"%d",-j);
            fprintf(fp,"%c",' ');
        }
        if((j%20) == 0)
           fprintf(fp,"%c",'\n');
        j++;
    }
    if((j-1)%20 != 0)
        fprintf(fp,"%c",'\n');
    fprintf(fp,"%c",'t');
    fprintf(fp,"%c",' ');
    fprintf(fp,"%lf",t);
    fprintf(fp,"%s","ms");
    return TRUE;
}


//ԭʼ���ԣ�ѡȡ�Ӿ��г��ִ������ı�Ԫ
status ChooseMaxBool(ALGraph G)
{
    int i;
    int *postive = (int *)malloc(sizeof(int) * (G.BoolElem+1)); //�洢������
    int *minus = (int *)malloc(sizeof(int) * (G.BoolElem+1));  //�洢������
    for(i = 1;i <= G.BoolElem;i++) //��ʼ������
    {
        postive[i] = 0;
        minus[i] = 0;
    }
    SentNode *s;
    s = G.FirstSent;
    if(s == NULL){
        return FALSE;
    }
    BoolNode *b;
    b = s->FirstBool->nextbool;
    if(s->FirstBool == NULL)
    {
        return FALSE;
    }
    while(s)                        //����cnf�ļ�����ȡ��Ԫ���ֵĴ���
    {
        while(b)
        {
            if((b->data) > 0)
                postive[b->data]++;
            if((b->data) < 0)
                minus[-(b->data)]++;
            b = b->nextbool;
        }
        if(s->nextsent==NULL)
            break;
        s = s->nextsent;
        b = s->FirstBool->nextbool;
    }
    int p = 0,m = 0;
    for(i = 1;i <= G.BoolElem;i++)   //�ҵ����ֵ�������
    {
        if(p < postive[i])
            p = postive[i];
        if(m < minus[i])
            m = minus[i];
    }
    for(i = 1;i <= G.BoolElem;i++)   //�ҵ����ִ������ı�Ԫ���
    {
        if(p == postive[i])
        {
            p = i;
            break;
        }
    }
    for(i = 1;i <= G.BoolElem;i++)
    {
        if(m == minus[i])
        {
            m = i;
            break;
        }
    }
    if(p > m)
        return p;
    else
        return -m;
}

status DPLL_ChooseMax(ALGraph *G)
{
    int unisent;
    ALGraph *R,*cop;
    R = G;
    SentNode *s;
    s = R->FirstSent;
    if(s == NULL)
    {
        return TRUE;
    }

    while(FindUniSent(R) != 0)               //���Ӿ���򻯼�
    {
        unisent = FindUniSent(R);
        if(unisent > 0)
            value[abs(unisent)] = 1;
        else
            value[abs(unisent)] = 0;
        DeleteSent(R,unisent);
        DeleteNoBool(R,unisent);
        if(G->FirstSent == NULL)
        {
            return TRUE;
        }
        if(FindEmptySent(R))
        {
            return FALSE;
        }

    }
    if(ChooseMaxBool(*R)){
        unisent = ChooseMaxBool(*R);
    }
    CopyCnf(R,&cop);
    InsertUniSent(R,unisent);
    if(DPLL_ChooseMax(R))
    {
        return TRUE;
    }
    else
    {
        InsertUniSent(cop,-unisent);
        return DPLL_ChooseMax(cop);
    }
}

//�Ż�����2��ʹ��MOM�㷨ѡȡ��Ԫ
status Mom_ChooseBool(ALGraph G)
{
    int i,pi,mi;
    double *postive = (double *)malloc(sizeof(double) * (G.BoolElem+1)); //�洢������
    double *minus = (double *)malloc(sizeof(double) * (G.BoolElem+1));  //�洢������
    for(i = 1;i <= G.BoolElem;i++) //��ʼ������
    {
        postive[i] = 0;
        minus[i] = 0;
    }
    SentNode *s;
    s = G.FirstSent;
    if(s == NULL){
        return FALSE;
    }
    BoolNode *b;
    b = s->FirstBool->nextbool;
    if(s->FirstBool == NULL)
    {
        return FALSE;
    }
    while(s)                        //����cnf�ļ�����ȡ��Ԫ���ֵĴ���
    {
        while(b)
        {
            if((b->data) > 0)
                postive[b->data] = postive[b->data] + pow(2,0-s->data);
            if((b->data) < 0)
                minus[-(b->data)] = minus[-(b->data)] + pow(2,0-s->data);
            b = b->nextbool;
        }
        if(s->nextsent==NULL)
            break;
        s = s->nextsent;
        b = s->FirstBool->nextbool;
    }
    double p = 0,m = 0;
    for(i = 1;i <= G.BoolElem;i++)   //�ҵ����ֵ�������
    {
        if(p < postive[i])
            p = postive[i];
        if(m < minus[i])
            m = minus[i];
    }
    for(i = 1;i <= G.BoolElem;i++)   //�ҵ����ִ������ı�Ԫ���
    {
        if(p == postive[i])
        {
            pi = i;
            break;
        }
    }
    for(i = 1;i <= G.BoolElem;i++)
    {
        if(m == minus[i])
        {
            mi = i;
            break;
        }
    }
    if(p > m)
        return pi;
    else
        return -mi;
}

status DPLL_MomChoose(ALGraph *G)
{
    int unisent;
    ALGraph *R,*cop;
    R = G;
    SentNode *s;
    s = R->FirstSent;
    if(s == NULL)
    {
        return TRUE;
    }

    while(FindUniSent(R) != 0)               //���Ӿ���򻯼�
    {
        unisent = FindUniSent(R);
        if(unisent > 0)
            value[abs(unisent)] = 1;
        else
            value[abs(unisent)] = 0;
        DeleteSent(R,unisent);
        DeleteNoBool(R,unisent);
        if(G->FirstSent == NULL)
        {
            return TRUE;
        }
        if(FindEmptySent(R))
        {
            return FALSE;
        }

    }
    if(Mom_ChooseBool(*R)){
        unisent = Mom_ChooseBool(*R);
        //printf("Max %d  ",unisent);
    }
    CopyCnf(R,&cop);
    InsertUniSent(R,unisent);
    //printf("Insert %d  ",unisent);
    if(DPLL_MomChoose(R))
    {
        return TRUE;
    }
    else
    {
        InsertUniSent(cop,-unisent);
        return DPLL_MomChoose(cop);
    }
}














