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
        printf("1.cnf文件读取;\n");
        printf("2.DPLL求解:选取出现次数最多的变元;\n");
        printf("3.随机生成数独并求解;\n");
        printf("4.优化策略1：选取当前第一个变元;\n");
        printf("5.优化策略2：MOM算法;\n");
        printf("6.验证是否正确;\n");
        printf("0.退出程序;\n");
        printf("-----------------------------\n");
        printf("请输入你的操作:\n");
        scanf("%d",&op);
        char resultname[100];
        char number[20] = "EmptySudoku";
        //char shudu[20] = "shudu";
        if(op == 0)
            exit(0);
        if(op < 0 || op > 7)
        {
            printf("请选择正确的操作！");
            getchar();getchar();
            continue;
        }
        if(G.BoolElem == 0 && op != 1)
        {
            printf("请先读取cnf文件!\n");
            getchar();getchar();
            continue;
        }

        switch(op)
        {
        case 1:
            printf("请输入你要读取的文件名字: ");
            scanf("%s",filename);
            strcat(filename,".cnf");
            if((fp=fopen(filename,"r"))==NULL)
	        {
	            printf("打开文件失败!\n ");
	            fclose(fp);
	            getchar();getchar();
		        break;
	        }
            ReadDnfFile(fp,&G);
            printf("变元数目：%d\n子句数目：%d\n",G.BoolElem,G.Sent);
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
                printf("不满足!\n");
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
	            printf("打开文件失败!\n ");
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
            printf("运用sat求解出的数独结果;\n");
            InitValue(&Su);
            start = clock();
            if(DPLL(&Su)){
                stop = clock();
                t = ((double)(stop - start))/CLK_TCK;
                printf("%lfms\n",t*1000);
            }
            else
                printf("不满足!\n");
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
                printf("不满足!\n");
                printf("%lfms\n",t*1000);
            }
            CopyName(filename,resultname);
            strcat(resultname,".res");
            if((fp=fopen(resultname,"w"))==NULL)
	        {
	            printf("创建文件失败!\n ");
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
                printf("不满足!\n");
                printf("%lfms\n",t*1000);
            }

            getchar();getchar();
		    break;
        case 6:
            if((fp=fopen(filename,"r"))==NULL)
	        {
	            printf("打开文件失败!\n ");
	            fclose(fp);
	            getchar();getchar();
		        break;
	        }
            ReadDnfFile(fp,&G);
            fclose(fp);
            PrintValue(&G);
            printf("\n----------------------------------------\n");
            CheckResult(&G);
            printf("变元数目：%d\n子句数目：%d\n",G.BoolElem,G.Sent);
            getchar();getchar();
            break;
        }
    }
    printf("再见!\n");
    return 0;
}
