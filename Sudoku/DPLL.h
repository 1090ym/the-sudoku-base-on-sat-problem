#ifndef DPLL_H
#define DPLL_H
#include "CNFfile.h"
#include <stdio.h>

status DeleteSent(ALGraph *G,int b);                            //删除包含该变元的子句
status InsertUniSent(ALGraph *G,int b);                         //插入一个单子句
status FindUniSent(ALGraph *G);                                 //找到单子句，并获取该单子句的变元值
status DeleteNoBool(ALGraph *G,int b);                          //删除相反的变元
status ChooseMaxBool(ALGraph G);                                //选取子句中出现次数最多的变元
status DPLL_ChooseMax(ALGraph *G);                              //原始策略：选取子句中出现次数最多的变元
status FindEmptySent(ALGraph *G);                               //判断是否存在空子句
status FindBool(SentNode *s,int boo);                           //判断一个子句中是否存在该变元
status CopyCnf(ALGraph *G,ALGraph **Copy);                      //复制一个cnf结构
status PrintValue(ALGraph *G);                                  //打印出变元的真值
status InitValue(ALGraph *G);                                   //初始化全局变量
status WriteCnfValue(FILE *fp,int result,int Bool,double t);    //将cnf文件的求解结果写入文件
status DPLL(ALGraph *G);                                        //使用策略1进行优化的DPLL算法：直接选取第一个子句的第一个变元
status Mom_ChooseBool(ALGraph G);                               //使用MOM算法选取变元
status DPLL_MomChoose(ALGraph *G);                              //优化策略2：使用MOM算法选取变元
status CheckResult(ALGraph *G);

#endif // DPLL_H
