#ifndef SUDOKU_H
#define SUDOKU_H
#include "CNFfile.h"
#include "DPLL.h"
#include <stdio.h>

status set(int x, int y, int val);
void reset(int x, int y);
void initXOrd(int* xOrd);
status fillForm(int y, int val);
void digHole(int holeCnt);
void printSudo();
status CreateSudo();
status set_as0();

status entry_atmost_once(FILE *fp);
status row_atleast_once(FILE *fp);
status col_atleast_once(FILE *fp);
status three_square_atleast_once(FILE *fp);
status sudo_tran_cnf(char *filename,FILE *fp);
status sudo_problem(ALGraph *G);
status PrintSu();
status cnf_tran_sudo();


#endif // DPLL_H
