#ifndef DPLL_H
#define DPLL_H
#include "CNFfile.h"
#include <stdio.h>

status DeleteSent(ALGraph *G,int b);                            //ɾ�������ñ�Ԫ���Ӿ�
status InsertUniSent(ALGraph *G,int b);                         //����һ�����Ӿ�
status FindUniSent(ALGraph *G);                                 //�ҵ����Ӿ䣬����ȡ�õ��Ӿ�ı�Ԫֵ
status DeleteNoBool(ALGraph *G,int b);                          //ɾ���෴�ı�Ԫ
status ChooseMaxBool(ALGraph G);                                //ѡȡ�Ӿ��г��ִ������ı�Ԫ
status DPLL_ChooseMax(ALGraph *G);                              //ԭʼ���ԣ�ѡȡ�Ӿ��г��ִ������ı�Ԫ
status FindEmptySent(ALGraph *G);                               //�ж��Ƿ���ڿ��Ӿ�
status FindBool(SentNode *s,int boo);                           //�ж�һ���Ӿ����Ƿ���ڸñ�Ԫ
status CopyCnf(ALGraph *G,ALGraph **Copy);                      //����һ��cnf�ṹ
status PrintValue(ALGraph *G);                                  //��ӡ����Ԫ����ֵ
status InitValue(ALGraph *G);                                   //��ʼ��ȫ�ֱ���
status WriteCnfValue(FILE *fp,int result,int Bool,double t);    //��cnf�ļ��������д���ļ�
status DPLL(ALGraph *G);                                        //ʹ�ò���1�����Ż���DPLL�㷨��ֱ��ѡȡ��һ���Ӿ�ĵ�һ����Ԫ
status Mom_ChooseBool(ALGraph G);                               //ʹ��MOM�㷨ѡȡ��Ԫ
status DPLL_MomChoose(ALGraph *G);                              //�Ż�����2��ʹ��MOM�㷨ѡȡ��Ԫ
status CheckResult(ALGraph *G);

#endif // DPLL_H
