#include "CNFfile.h"
#include "DPLL.h"
#include "sudoku.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern int value[1100];
int sudo[9][9],hole[9][9],sudo_solve[9][9];
int count = 0;

//在数独中填入满足规则的数
status set(int x, int y, int val)
{
    if (sudo[y][x] != 0)        //非空
        return FALSE;
    int x0, y0;
    for (x0=0; x0<9; x0++)
    {
        if (sudo[y][x0] == val) //行冲突
            return FALSE;
    }
    for (y0=0; y0<9; y0++)
    {
        if (sudo[y0][x] == val) //列冲突
            return FALSE;
    }
    for (y0=y/3*3; y0<y/3*3+3; y0++)
    {
        for (x0=x/3*3; x0<x/3*3+3; x0++)
        {
            if (sudo[y0][x0] == val) //格冲突
                return FALSE;
        }
    }
    sudo[y][x] = val;
    return TRUE;
}

//置为零
void reset(int x, int y)
{
    sudo[y][x] = 0;
}

//初始化0-8的随机位置序列
void initXOrd(int* xOrd)
{
    int i, k, tmp;
    for (i=0; i<9; i++)
    {
        xOrd[i] = i;           //按顺序生成0-8
    }
    for (i=0; i<9; i++)
    {
        k = rand() % 9;        //生成0-8的随机数
        tmp = xOrd[k];
        xOrd[k] = xOrd[i];
        xOrd[i] = tmp;
    }
}

//填满数独空格：按顺序在每一行不同的位置填上1-9
status fillForm(int y, int val)
{
    int i;
    int xOrd[9];
    initXOrd(xOrd);     //生成当前行的随机列数序列
    for (i=0; i<9; i++)
    {
        int x = xOrd[i];
        if (set(x, y, val))
        {
            if (y == 8)                 //到了最后一行
            {
                //当前填9则结束, 否则从第一行填下一个数
                if (val == 9 || fillForm(0, val+1))
                    return TRUE;
            }
            else
            {
                if (fillForm(y+1, val)) //下一行继续填当前数
                    return TRUE;
            }
            reset(x, y);    //如果矛盾，则回溯
        }
    }
    return FALSE;
}

//对数独进行随机挖洞
void digHole(int holeCnt)
{
    int idx[81];
    int i, k;
    for (i=0; i<81; i++)
    {
        hole[i / 9][i % 9] = 0;       //按顺序初始化数组
        idx[i] = i;
    }
    for (i=0; i<holeCnt; i++)    //确定随机挖洞位置
    {
        k = rand() % 81;
        int tmp = idx[k];
        idx[k] = idx[i];
        idx[i] = tmp;
    }
    for (i=0; i<holeCnt; i++)
    {
        hole[idx[i] / 9][idx[i] % 9] = 1;      //挖洞的位置标记为1
    }
}

//打印数独
void printSudo()
{
    int y,x;
    for (y=0; y<9; y++)
    {
        (y % 3 == 0) ? (printf("------------------------\n ")) : (printf(" "));
        for (x=0; x<9; x++)
        {
            (hole[y][x] == 0) ? (printf("%d ",sudo[y][x])) : (printf("  "));
            (x % 3 == 2) ? (printf("| ")) : (printf(""));
        }
        printf("\n");
    }
    printf("------------------------\n");
}

//将存储数独的数组全部清空
status set_as0()
{
    int i,j;
    for(i = 0;i < 9;i++)
    {
        for(j = 0;j < 9;j++)
        {
            sudo[i][j] = 0;
            hole[i][j] = 0;
        }
    }
    return TRUE;
}

//创建数独
status CreateSudo()
{
    srand((unsigned)time(NULL));
    fillForm(0,1);                 //生成完整的数独
    digHole(30);                   //对数独进行随机挖洞
    printSudo();                   //打印数独
    return TRUE;
}

//将数独转化为sat问题，并采用DPLL求解器进行求解
//用x,y,z表示数独的坐标(x,y)和填z值
//x,y,z对应变元81*(x-1)+9*(y-1)+z;


//使数独中的一个空格最多填一个数，保证同一个位置只能填一个数
status entry_atmost_once(FILE *fp)
{
    int i,x,y,z;
    int first,second;
    for(x = 1;x < 10;x++)
        for(y = 1;y < 10;y++)
            for(z = 1;z < 9;z++)
                for(i = z+1;i < 10;i++)
                {
                    first = 81*(x-1) + 9*(y-1) +(z-1) + 1;    //将变元编码为对应的数字
                    second = 81*(x-1) + 9*(y-1) +(i-1) + 1;
                    fprintf(fp,"%d",-first);
                    fprintf(fp,"%c",' ');
                    fprintf(fp,"%d",-second);
                    fprintf(fp,"%c",' ');
                    fprintf(fp,"%d",0);
                    fprintf(fp,"%c",'\n');
                    count++;
                }
    return TRUE;
}

//每个数在每一行至少出现一次，保证同一行不出现相同的数
status row_atleast_once(FILE *fp)
{
    int x,y,z;
    int input;
    for(y = 1;y < 10;y++)
        for(z = 1;z < 10;z++)
        {
            for(x = 1;x < 10;x++)
                {
                    input = 81*(x-1) + 9*(y-1) +(z-1) + 1;    //将变元编码为对应的数字

                    fprintf(fp,"%d",input);
                    fprintf(fp,"%c",' ');
                }
            fprintf(fp,"%d",0);
            fprintf(fp,"%c",'\n');
            count++;
        }
    return TRUE;
}

//每个数在每一列至少出现一次，保证同一列不出现相同的数
status col_atleast_once(FILE *fp)
{
    int x,y,z;
    int input;
    for(x = 1;x < 10;x++)
        for(z = 1;z < 10;z++)
        {
            for(y = 1;y < 10;y++)
                {
                    input = 81*(x-1) + 9*(y-1) +(z-1) + 1;    //将变元编码为对应的数字

                    fprintf(fp,"%d",input);
                    fprintf(fp,"%c",' ');
                }
            fprintf(fp,"%d",0);
            fprintf(fp,"%c",'\n');
            count++;
        }
    return TRUE;
}

//每个数在一个3*3的单元里至少出现一次，保证同一个单元里不出现相同的数
status three_square_atleast_once(FILE *fp)
{
    int x,y,z,i,j;
    int input;
    for(z = 1;z < 10;z++)
        for(i = 0;i < 3;i++)
            for(j = 0;j < 3;j++)
            {
                for(x = 1;x < 4;x++)
                    for(y = 1;y < 4;y++)
                    {
                        input = 81*((3*i+x)-1) + 9*((3*j+y)-1) +(z-1) + 1;    //将变元编码为对应的数字

                        fprintf(fp,"%d",input);
                        fprintf(fp,"%c",' ');
                    }
                fprintf(fp,"%d",0);
                fprintf(fp,"%c",'\n');
                count++;
            }
    return TRUE;
}

//将一个全空的数独写入一个cnf文件
status sudo_tran_cnf(char *filename,FILE *fp)
{
    int bo;
    bo = 729;
    fprintf(fp,"%c",'c');
    fprintf(fp,"%c",' ');
    fprintf(fp,"%s",filename);
    fprintf(fp,"%c",'\n');
    fprintf(fp,"%c",'p');
    fprintf(fp,"%c",' ');
    fprintf(fp,"%s","cnf");
    fprintf(fp,"%c",' ');
    fprintf(fp,"%d",bo);
    fprintf(fp,"%c",' ');
    fprintf(fp,"%d",3159);
    fprintf(fp,"%c",'\n');
    entry_atmost_once(fp);
    row_atleast_once(fp);
    col_atleast_once(fp);
    three_square_atleast_once(fp);
    return TRUE;
}

//将一个全空的数独转换为填好了一部分空的数独问题
status sudo_problem(ALGraph *G)
{
    if(G->FirstSent == NULL)
    {
        return OVERFLOW;
    }
    ALGraph *R;
    R = G;
    int i,j;
    int bo;

    for(i = 0;i < 9;i++)
        for(j = 0;j < 9;j++)
        {
            if(hole[i][j] == 0)
            {
                bo = 81*i + 9*j +(sudo[i][j]-1) + 1;  //找到已经填好的空，并转换为变元
                DeleteSent(R,bo);                     //删除包含该变元的子句
                DeleteNoBool(R,bo);                   //删除与该变元相反的变元
            }
        }
    return TRUE;
}

//打印出通过DPLL求解出来的数独
status PrintSu()
{
    int y,x;
    for (y=0; y<9; y++)
    {
        (y % 3 == 0) ? (printf("------------------------\n ")) : (printf(" "));
        for (x=0; x<9; x++)
        {
            printf("%d ",sudo_solve[y][x]);
            (x % 3 == 2) ? (printf("| ")) : (printf(""));
        }
        printf("\n");
    }
    printf("------------------------\n");
    return TRUE;
}

//将通过DPLL求解出来的答案转换为数独的答案
status cnf_tran_sudo()
{
    int i,j;
    int bo;
    int x,y,z;

    for(i = 0;i < 800;i++)
    {
        if(value[i] == 1)
        {
            x = i/81;
            y = (i-81*x)/9;
            z = i - x*81 - y*9 ;
            if( z == 0 )
                z = 9;
            sudo_solve[x][y] = z;
        }
    }

    for(i = 0;i < 9;i++)
    {
        for(j = 0;j < 9;j++)
        {
            if(hole[i][j] == 0)
            {
                sudo_solve[i][j] = sudo[i][j];
            }
        }
    }

    for(i = 0;i < 9;i++)
    {
        for(j = 0;j < 9;j++)
        {
            if(sudo_solve[i][j] == 0)
                sudo_solve[i][j] = 9;
        }
    }
    return TRUE;
}


