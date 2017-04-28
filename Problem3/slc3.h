// lc3.h
#include <stdio.h>
#include <stdlib.h>
#define FETCH 0
#define DECODE 1
#define EVAL_ADDR 2
#define FETCH_OP 4
#define EXECUTE 5
#define STORE 6

// instructions
#define ADD 1
#define AND 5
#define NOT 9
#define TRAP 15
#define LD 2
#define LDR 6
#define ST 3
#define JMP 12
#define BR 0
// etc.

#define SEXT6_SIGN_EXTEND 0xFFC0
#define HIGH_ORDER_BIT_VALUE9 0x0100

#define SEXT9_SIGN_EXTEND 0xFE00
#define HIGH_ORDER_BIT_VALUE9 0x0100


typedef unsigned short Register;
//cpu a b res mar mdr
// lc3.c
typedef struct CPU_s{
	Register r[8];
	Register A, B, Res;
	Register PC, ir;
	Register MAR, MDR;
	Register N, Z, P;
	Register ImmFlag;
	Register IMM5;
} CPU_s, *CPU_p;

void trap(int vector){
	switch(vector){
		case 25:
			exit(0);
			break;
	}
}
