// lc3.h
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <termios.h> 


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
#define LEA 14
#define ST 3
#define JMP 12
#define JSRR 4
#define BR 0

// traps
#define HALT 0x25
#define GETC 0x20
#define OUT 0x21
#define PUTS 0x22

// etc.
#define SEXT6_SIGN_EXTEND 0xFFC0
#define HIGH_ORDER_BIT_VALUE6 0x0020

#define SEXT9_SIGN_EXTEND 0xFE00
#define HIGH_ORDER_BIT_VALUE9 0x0100

#define MAX_MEMORY 500


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
	char *out;
} CPU_s, *CPU_p;
