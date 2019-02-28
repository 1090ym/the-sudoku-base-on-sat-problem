#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "CNFfile.h"
#include "DPLL.h"
#include "sudoku.h"

#define TRUE 1
#define OK 1
#define FALSE 0
#define ERROR 0
#define OVERFLOE -2
#define MAX_VERTEX_NUM 100
typedef int status;
typedef int ElemType;

int main()
{
    int res,BoolNum;
    clock_t start,stop,end;
    double t;
    ALGraph G;
    SentNode S;
    G.FirstSent = &S;
    FILE *fp = NULL;
    char filename[100];
    int op = 1;
    while(op){
        ALGraph Su;
        SentNode s;
        Su.FirstSent = &s;
        system("cls");  printf(" \n\n");
        printf("-----------------------------\n");
        printf("1.cnf�ļ���ȡ;\n");
        printf("2.DPLL���:ѡȡ���ִ������ı�Ԫ;\n");
        printf("3.����������������;\n");
        printf("4.�Ż�����1��ѡȡ��ǰ��һ����Ԫ;\n");
        printf("5.�Ż�����2��MOM�㷨;\n");
        printf("6.��֤�Ƿ���ȷ;\n");
        printf("0.�˳�����;\n");
        printf("-----------------------------\n");
        printf("��������Ĳ���:\n");
        scanf("%d",&op);
        char resultname[100];
        char number[20] = "EmptySudoku";
        //char shudu[20] = "shudu";
        if(op == 0)
            exit(0);
        if(op < 0 || op > 7)
        {
            printf("��ѡ����ȷ�Ĳ�����");
            getchar();getchar();
            continue;
        }
        if(G.BoolElem == 0 && op != 1)
        {
            printf("���ȶ�ȡcnf�ļ�!\n");
            getchar();getchar();
            continue;
        }

        switch(op)
        {
        case 1:
            printf("��������Ҫ��ȡ���ļ�����: ");
            scanf("%s",filename);
            strcat(filename,".cnf");
            if((fp=fopen(filename,"r"))==NULL)
	        {
	            printf("���ļ�ʧ��!\n ");
	            fclose(fp);
	            getchar();getchar();
		        break;
	        }
            ReadDnfFile(fp,&G);
            printf("��Ԫ��Ŀ��%d\n�Ӿ���Ŀ��%d\n",G.BoolElem,G.Sent);
            fclose(fp);
            getchar();getchar();
		    break;

        case 2:
            printf("--------------------------------\n");
            InitValue(&G);
            start = clock();
            if(DPLL_ChooseMax(&G)){
                stop = clock();
                PrintValue(&G);
                t = ((double)(stop - start))/CLK_TCK;
                printf("%lfms\n",t*1000);
            }
            else
            {
                end = clock();
                t = ((double)(end - start))/CLK_TCK;
                printf("������!\n");
                printf("%lfms\n",t*1000);
            }
            getchar();getchar();
		    break;

        case 3:
            set_as0();
            CreateSudo();
            strcat(number,".cnf");
            if((fp=fopen(number,"r"))==NULL)
	        {
	            printf("���ļ�ʧ��!\n ");
	            fclose(fp);
	            getchar();getchar();
		        break;
	        }
	        //sudo_tran_cnf(number,fp);
            ReadDnfFile(fp,&Su);
            fclose(fp);
            sudo_problem(&Su);
            /*TarverseBool(&Su);
            getchar();  getchar();
            system("cls");

            printSudo();*/
            printf("����sat�������������;\n");
            InitValue(&Su);
            start = clock();
            if(DPLL(&Su)){
                stop = clock();
                t = ((double)(stop - start))/CLK_TCK;
                printf("%lfms\n",t*1000);
            }
            else
                printf("������!\n");
            cnf_tran_sudo();
            PrintSu();
            getchar();getchar();
		    break;
        case 4:
            printf("--------------------------------\n");
            InitValue(&G);
            BoolNum = G.BoolElem;
            start = clock();
            if(DPLL(&G)){
                stop = clock();
                res = 1;
                PrintValue(&G);
                t = ((double)(stop - start))/CLK_TCK;
                printf("%lfms\n",t*1000);
            }
            else
            {
                end = clock();
                t = ((double)(end - start))/CLK_TCK;
                printf("������!\n");
                printf("%lfms\n",t*1000);
            }
            CopyName(filename,resultname);
            strcat(resultname,".res");
            if((fp=fopen(resultname,"w"))==NULL)
	        {
	            printf("�����ļ�ʧ��!\n ");
	            fclose(fp);
	            getchar();getchar();
		        break;
	        }
            WriteCnfValue(fp,res,BoolNum,t);
            fclose(fp);
            getchar();getchar();
		    break;
        case 5:
            printf("--------------------------------\n");
            InitValue(&G);
            start = clock();
            if(DPLL_MomChoose(&G)){
                stop = clock();
                PrintValue(&G);
                t = ((double)(stop - start))/CLK_TCK;
                printf("%lfms\n",t*1000);
            }
            else
            {
                end = clock();
                t = ((double)(end - start))/CLK_TCK;
                printf("������!\n");
                printf("%lfms\n",t*1000);
            }

            getchar();getchar();
		    break;
        case 6:
            if((fp=fopen(filename,"r"))==NULL)
	        {
	            printf("���ļ�ʧ��!\n ");
	            fclose(fp);
	            getchar();getchar();
		        break;
	        }
            ReadDnfFile(fp,&G);
            fclose(fp);
            PrintValue(&G);
            printf("\n----------------------------------------\n");
            CheckResult(&G);
            printf("��Ԫ��Ŀ��%d\n�Ӿ���Ŀ��%d\n",G.BoolElem,G.Sent);
            getchar();getchar();
            break;
        }
    }
    printf("�ټ�!\n");
    return 0;
}
