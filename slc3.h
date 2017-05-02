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
#define STR 7
#define JMP 12 //RET is not labelled because it is only a special case of JMP
#define JSRR 4
#define BR 0

// traps
#define HALT 0x25
#define GETC 0x20
#define OUT 0x21
#define PUTS 0x22

// user choices
#define LOAD 1
#define STEP 3
#define DISP_MEM 5
#define RUN 7
#define EXIT 9

// etc.
#define SEXT6_SIGN_EXTEND 0xFFC0
#define SEXT6_ZERO_EXTEND 0x003F
#define HIGH_ORDER_BIT_VALUE6 0x0020

#define SEXT9_SIGN_EXTEND 0xFE00
#define HIGH_ORDER_BIT_VALUE9 0x0100

#define SHIFT_OPCODE 12

#define RETRIEVE_RD 0x0FFF
#define SHIFT_RD 9

#define RETRIEVE_RS1 0x01FF
#define SHIFT_RS1 6

#define RETRIEVE_RS2 0x0007

#define RETRIEVE_BASE 0x01C0

#define RETRIEVE_TRAP_VECTOR 0x00FF

#define IMMED_OFFSET_MASK 0x001F

#define N_BIT 4
#define Z_BIT 2
#define P_BIT 1

#define CONVERT_TO_DECIMAL 0x2FFF

#define TOP_HALF 0x3000		//Used to show the values in memory on the screen.
#define BOTTOM_HALF 0x3008	//This was split up into two halves for clarity.

#define REG_FILE_SIZE 8
#define MAX_FILE_NAME 100
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
	char gotC;
} CPU_s, *CPU_p;
