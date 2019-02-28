#include "CNFfile.h"
#include "DPLL.h"
#include "sudoku.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern int value[1100];
int sudo[9][9],hole[9][9],sudo_solve[9][9];
int count = 0;

//����������������������
status set(int x, int y, int val)
{
    if (sudo[y][x] != 0)        //�ǿ�
        return FALSE;
    int x0, y0;
    for (x0=0; x0<9; x0++)
    {
        if (sudo[y][x0] == val) //�г�ͻ
            return FALSE;
    }
    for (y0=0; y0<9; y0++)
    {
        if (sudo[y0][x] == val) //�г�ͻ
            return FALSE;
    }
    for (y0=y/3*3; y0<y/3*3+3; y0++)
    {
        for (x0=x/3*3; x0<x/3*3+3; x0++)
        {
            if (sudo[y0][x0] == val) //���ͻ
                return FALSE;
        }
    }
    sudo[y][x] = val;
    return TRUE;
}

//��Ϊ��
void reset(int x, int y)
{
    sudo[y][x] = 0;
}

//��ʼ��0-8�����λ������
void initXOrd(int* xOrd)
{
    int i, k, tmp;
    for (i=0; i<9; i++)
    {
        xOrd[i] = i;           //��˳������0-8
    }
    for (i=0; i<9; i++)
    {
        k = rand() % 9;        //����0-8�������
        tmp = xOrd[k];
        xOrd[k] = xOrd[i];
        xOrd[i] = tmp;
    }
}

//���������ո񣺰�˳����ÿһ�в�ͬ��λ������1-9
status fillForm(int y, int val)
{
    int i;
    int xOrd[9];
    initXOrd(xOrd);     //���ɵ�ǰ�е������������
    for (i=0; i<9; i++)
    {
        int x = xOrd[i];
        if (set(x, y, val))
        {
            if (y == 8)                 //�������һ��
            {
                //��ǰ��9�����, ����ӵ�һ������һ����
                if (val == 9 || fillForm(0, val+1))
                    return TRUE;
            }
            else
            {
                if (fillForm(y+1, val)) //��һ�м����ǰ��
                    return TRUE;
            }
            reset(x, y);    //���ì�ܣ������
        }
    }
    return FALSE;
}

//��������������ڶ�
void digHole(int holeCnt)
{
    int idx[81];
    int i, k;
    for (i=0; i<81; i++)
    {
        hole[i / 9][i % 9] = 0;       //��˳���ʼ������
        idx[i] = i;
    }
    for (i=0; i<holeCnt; i++)    //ȷ������ڶ�λ��
    {
        k = rand() % 81;
        int tmp = idx[k];
        idx[k] = idx[i];
        idx[i] = tmp;
    }
    for (i=0; i<holeCnt; i++)
    {
        hole[idx[i] / 9][idx[i] % 9] = 1;      //�ڶ���λ�ñ��Ϊ1
    }
}

//��ӡ����
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

//���洢����������ȫ�����
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

//��������
status CreateSudo()
{
    srand((unsigned)time(NULL));
    fillForm(0,1);                 //��������������
    digHole(30);                   //��������������ڶ�
    printSudo();                   //��ӡ����
    return TRUE;
}

//������ת��Ϊsat���⣬������DPLL������������
//��x,y,z��ʾ����������(x,y)����zֵ
//x,y,z��Ӧ��Ԫ81*(x-1)+9*(y-1)+z;


//ʹ�����е�һ���ո������һ��������֤ͬһ��λ��ֻ����һ����
status entry_atmost_once(FILE *fp)
{
    int i,x,y,z;
    int first,second;
    for(x = 1;x < 10;x++)
        for(y = 1;y < 10;y++)
            for(z = 1;z < 9;z++)
                for(i = z+1;i < 10;i++)
                {
                    first = 81*(x-1) + 9*(y-1) +(z-1) + 1;    //����Ԫ����Ϊ��Ӧ������
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

//ÿ������ÿһ�����ٳ���һ�Σ���֤ͬһ�в�������ͬ����
status row_atleast_once(FILE *fp)
{
    int x,y,z;
    int input;
    for(y = 1;y < 10;y++)
        for(z = 1;z < 10;z++)
        {
            for(x = 1;x < 10;x++)
                {
                    input = 81*(x-1) + 9*(y-1) +(z-1) + 1;    //����Ԫ����Ϊ��Ӧ������

                    fprintf(fp,"%d",input);
                    fprintf(fp,"%c",' ');
                }
            fprintf(fp,"%d",0);
            fprintf(fp,"%c",'\n');
            count++;
        }
    return TRUE;
}

//ÿ������ÿһ�����ٳ���һ�Σ���֤ͬһ�в�������ͬ����
status col_atleast_once(FILE *fp)
{
    int x,y,z;
    int input;
    for(x = 1;x < 10;x++)
        for(z = 1;z < 10;z++)
        {
            for(y = 1;y < 10;y++)
                {
                    input = 81*(x-1) + 9*(y-1) +(z-1) + 1;    //����Ԫ����Ϊ��Ӧ������

                    fprintf(fp,"%d",input);
                    fprintf(fp,"%c",' ');
                }
            fprintf(fp,"%d",0);
            fprintf(fp,"%c",'\n');
            count++;
        }
    return TRUE;
}

//ÿ������һ��3*3�ĵ�Ԫ�����ٳ���һ�Σ���֤ͬһ����Ԫ�ﲻ������ͬ����
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
                        input = 81*((3*i+x)-1) + 9*((3*j+y)-1) +(z-1) + 1;    //����Ԫ����Ϊ��Ӧ������

                        fprintf(fp,"%d",input);
                        fprintf(fp,"%c",' ');
                    }
                fprintf(fp,"%d",0);
                fprintf(fp,"%c",'\n');
                count++;
            }
    return TRUE;
}

//��һ��ȫ�յ�����д��һ��cnf�ļ�
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

//��һ��ȫ�յ�����ת��Ϊ�����һ���ֿյ���������
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
                bo = 81*i + 9*j +(sudo[i][j]-1) + 1;  //�ҵ��Ѿ���õĿգ���ת��Ϊ��Ԫ
                DeleteSent(R,bo);                     //ɾ�������ñ�Ԫ���Ӿ�
                DeleteNoBool(R,bo);                   //ɾ����ñ�Ԫ�෴�ı�Ԫ
            }
        }
    return TRUE;
}

//��ӡ��ͨ��DPLL������������
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

//��ͨ��DPLL�������Ĵ�ת��Ϊ�����Ĵ�
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


