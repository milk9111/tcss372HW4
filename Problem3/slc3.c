/*
 * Addison Sims
 * Daniel Ivanov
 */
#include "slc3.h"

int controller (CPU_p cpu);

int displayScreen(CPU_p cpu, int mem);

int dialog(CPU_p cpu);


// you can define a simple memory module here for this program
unsigned short memory[32];   // 32 words of memory enough to store simple program
int isLoaded;
int memShift;


int sext6(int offset6) {
	if (HIGH_ORDER_BIT_VALUE6 & offset6) return (offset6 | SEXT6_SIGN_EXTEND);
	else return offset6;
}


int sext9(int offset9) {
	if (HIGH_ORDER_BIT_VALUE9 & offset9) return (offset9 | SEXT9_SIGN_EXTEND);
	else return offset9;
}

int displayScreen(CPU_p cpu, int mem) {

	printf("\t\tWelcome to the LC-3 Simulator Simulator\n\n");
	printf("\t\tRegisters \t\t    Memory\n");
//	for (int i = 0x0000; i < 8; i++) {
//		if (i == 0) {
//			printf("\t\tR%d: x%04X \t\t x300%d: x%04X\n", i, cpu->r[i], i+mem, memory[i+1]);
//		} else {
//			printf("\t\tR%d: x%04X \t\t x300%d: x%04X\n", i, cpu->r[i], i+mem, memory[i+1]);
//		}
//	}
	int i = 0x3000 + mem;
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 0, cpu->r[0], i, memory[1]);

	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 1, cpu->r[1], i+1, memory[2]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 2, cpu->r[2], i+2, memory[3]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 3, cpu->r[3], i+3, memory[4]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 4, cpu->r[4], i+4, memory[5]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 5, cpu->r[5], i+5, memory[6]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 6, cpu->r[6], i+6, memory[7]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 7, cpu->r[7], i+7, memory[8]);

	i = 0x3008 + mem; // replace i with the mem dump number if you want.
	printf("\t\t\t\t\t x%X: x%04X\n",i, memory[9]);
	printf("\t\t\t\t\t x%X: x%04X\n",i+1, memory[10]);
	printf("\t\t\t\t\t x%X: x%04X\n",i+2, memory[11]);
	printf("\t\tPC:x%0.4X    IR:x%04X     x%X: x%04X\n",cpu->PC,cpu->ir,i+3, memory[12]);
	printf("\t\tA: x%04X    B: x%04X     x%X: x%04X\n",cpu->A,cpu->B,i+4, memory[13]);
	printf("\t\tMAR:x%04X  MDR:x%04X     x%X: x%04X\n",cpu->MAR + 0x2FFF,cpu->MDR,i+5, memory[14]);
	printf("\t\tCC: N: %d  Z: %01d P: %d      x%X: x%04X\n",cpu->N,cpu->Z,cpu->P,i+6, memory[15]);
	printf("\t\t\t\t\t x%X: x%04X\n",i+7, memory[16]);
	printf("  Select: 1)Load, 3)Step, 5)Display Mem, 9)Exit\n");
	return 0;
}


int dialog(CPU_p cpu) {
	//int isTrue = 0;
	int opNum = 0;
	char fileName[100];
	FILE* inputFile;
		while (opNum != 9) {
			scanf("%d", &opNum);
			switch (opNum) {
				case 1:
					printf("File Name: ");
					scanf("%s", &fileName);
					inputFile = fopen(fileName, "r");
					if (inputFile == NULL) {
						printf("DIDNT OPEN");
						break;
					}
					for(int i = 0; i < 31; i++) {
						fscanf(inputFile, "%04X", &memory[i]);
						if (i == 0) {
							cpu->PC = memory[0];
						}

					}
					isLoaded = 1;
					displayScreen(cpu, 0);
					fclose(inputFile);
					break;
				case 3:
					if (isLoaded == 1) {
						controller(cpu);
						opNum = 0;
					} else {
						printf("No file loaded!");
					}
					break;
				case 5:
					printf("How much would you like to shift?:");

					scanf("%d", &memShift);
					if(memShift > 17) {
						printf("Error: out of memory");
						memShift = 0;
						break;
					} else {
						displayScreen(cpu, memShift);
					}
					//printf("About to send to displayScreen()\n");

					//dialog(cpu);
					break;
				case 9:
					printf("Simulation Terminated.");
					break;
			}
		}
}
int controller (CPU_p cpu) {
    // check to make sure both pointers are not NULL
    // do any initializations here
		unsigned int state;
		short cc;
	unsigned int opcode, Rd, Rs1, Rs2, immed_offset, BaseR;	// fields for the IR
    state = FETCH;
		int j;
		//for(;;){
    for (j = 0;j < 6;j++) {
        switch (state) {
            case FETCH: // microstates 18, 33, 35 in the book
                //printf("Here in FETCH\n");
                // get memory[PC] into IR - memory is a global array
            	 cpu->MAR = (cpu->PC - 0x2FFF);
            	 cpu->PC++;	// increment PC

            	                //microstate 33
            	 cpu->MDR = memory[cpu->MAR];

            	                //microstate 35
            	 cpu->ir = cpu->MDR;
            	 cc = 0;
                // increment PC
                //printf("Contents of IR = %04X\n", cpu->ir);
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // put printf statements in each state and microstate to see that it is working
 				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


               state = DECODE;
          	 //printf("IM HERE ");

                //break;
            case DECODE:
						// microstate 32
                // get the fields out of the IR
            	//printf("IM HERE");
								opcode = cpu->ir >> 12;

								//0x0fff
								Rd = cpu->ir & 0x0fff;
								Rd = (short)Rd >> 9;

								//0x01ff
								Rs1 = cpu->ir & 0x01ff;
								Rs1 = (short)Rs1 >> 6;

								//0x0007
								Rs2 = cpu->ir & 0x0007;

								//0x01ff
								immed_offset = cpu->ir & 0x01ff;
								
								BaseR = cpu->ir & 0x01C0;

                // make sure opcode is in integer form
				// hint: use four unsigned int variables, opcode, Rd, Rs, and immed7
				// extract the bit fields from the IR into these variables
                state = EVAL_ADDR;
                //break;
            case EVAL_ADDR: // Look at the LD instruction to see microstate 2 example
                switch (opcode) {
									case LDR:
										cpu->MAR = cpu->r[ + 
									case LD:
										cpu->MAR = (cpu->PC - 0x2FFF) + sext9(immed_offset);
										//printf("PC: %d, IMM: %d", (cpu->PC - 0x2FF), immed_offset);
										//cpu->r[Rd] = memory[cpu->PC + immed-offset]
										break;
									case ST:
										cpu->MAR = (cpu->PC - 0x2FFF) + sext9(immed_offset);
										//memory[cpu->PC + immed-offset] = Rd
										break;
                // different opcodes require different handling
                // compute effective address, e.g. add sext(immed7) to register
                }
                state = FETCH_OP;
                //break;
            case FETCH_OP: // Look at ST. Microstate 23 example of getting a value out of a register
                switch (opcode) {
									case LD:
									//	printf("MAR: %d", cpu->MAR);
									  cpu->MDR = memory[cpu->MAR];
										break;
									case ADD:
										if(0x0020 & cpu->ir){ //0000|0000|0010|0000
											cpu->A = cpu->r[Rs1];
											cpu->B = (immed_offset & 0x001f);
										} else{
											cpu->A = cpu->r[Rs1];
											cpu->B = cpu->r[Rs2];
										}

										break;
									case AND:
									if(0x0020 & cpu->ir){ //0000|0000|0010|0000
											cpu->A = cpu->r[Rs1];
											cpu->B = (immed_offset & 0x001f);
										} else{
											cpu->A = cpu->r[Rs1];
											cpu->B = cpu->r[Rs2];
										}
										break;
									case NOT:
										cpu->A = cpu->r[Rs1];
										break;
									case ST:
										cpu->MDR = cpu->r[Rd];
                    // get operands out of registers into A, B of ALU
                    // or get memory for load instr.
										break;
                }
                state = EXECUTE;
                //break;
            case EXECUTE: // Note that ST does not have an execute microstate
                switch (opcode) {
                    // do what the opcode is for, e.g. ADD
                    // in case of TRAP: call trap(int trap_vector) routine, see below for TRAP x25 (HALT)
										case ADD:
											//printf("cpu->a is %X", (cpu->A));
											//printf("cpu->b is %X", (cpu->B));
											// checks for negative addition
											if (cpu->A & 0x10) {
												cpu->Res = -(cpu->A) + (cpu->B);
											} else if (cpu->B & 0x10) {
												cpu->Res = (cpu->A) + -(cpu->B);
											} else if ((cpu->A & 0x10) & (cpu->B & 0x10)) {
												cpu->Res = -(cpu->A) + -(cpu->B);
											} else {
												cpu->Res = (cpu->A) + (cpu->B);
											}
											cpu->N = 0;
											cpu->Z = 0;
											cpu->P = 0;
											//int g = sizeof(cpu->Res);
											cc = (short int) cpu->Res;
											//printf(" Check if cc is negative when it needs to be    %d", cc);
											if (cc < 0x0000){
												cpu->N = 1;
											}
											if (cc == 0x0000){
												cpu->Z = 1;
											}
											if (cc > 0x0000){
												cpu->P = 1;
											}
											break;
										case AND:
											cpu->Res = cpu->A & cpu->B;
											cpu->N = 0;
											cpu->Z = 0;
											cpu->P = 0;
											cc = cpu->Res;
											if (cc < 0x0000){
												cpu->N = 1;
											}
											if (cc == 0x0000){
												cpu->Z = 1;
											}
											if (cc > 0x0000){
												cpu->P = 1;
											}

											break;
										case NOT:
											cpu->Res = ~(cpu->A);
											cpu->N = 0;
											cpu->Z = 0;
											cpu->P = 0;
											cc = (short) cpu->Res;
											if (cc < 0x0000){
												cpu->N = 1;
											}
											if (cc == 0x0000){
												cpu->Z = 1;
											}
											if (cc > 0x0000){
												cpu->P = 1;
											}
											break;
										case TRAP:
											trap((int) (immed_offset & 0x00ff));
											exit(0);
											break;

										case JMP:
											cpu->PC = cpu->r[Rs1];
											break;
										case BR:
												if (cpu->N && (Rd & 4)) {
													cpu->PC = cpu->PC + sext9(immed_offset);
													break;
												}
												if (cpu->Z && (Rd & 2)) {
													cpu->PC = cpu->PC + sext9(immed_offset);
													break;
												}
												if (cpu->P && (Rd & 1)) {
													cpu->PC = cpu->PC + sext9(immed_offset);
													break;
												}
											break;
                }
                state = STORE;
                //break;
            case STORE: // Look at ST. Microstate 16 is the store to memory
                switch (opcode) {
									case ADD:
										cpu->r[Rd] = cpu->Res;

										break;
									case AND:
										cpu->r[Rd] = cpu->Res;
										break;
									case NOT:
										cpu->r[Rd] = cpu->Res;
										break;
									// case TRAP:
									case LD:
										cpu->r[Rd] = cpu->MDR;
										break;
									case ST:
										memory[cpu->MAR] = cpu->MDR;
									// case JMP:
									// case BR:

										break;
                    // write back to register or store MDR into memory
                }
                // do any clean up here in prep for the next complete cycle
                state = FETCH;
                break;
        }
        displayScreen(cpu, 0);
        dialog(cpu);
    }
//		int i = 0;
//		for(i = 0; i < 8;i++){
//			printf("R%d = %u\n",i, cpu->r[i]);
//		}
//		printf("MAR = %u\nIR = %u\nPC = %u\n",cpu->MAR, cpu->ir, cpu->PC);
}

int main(int argc, char* argv[]){

//	char *temp;
//	int i;
	setvbuf(stdout, NULL, _IONBF, 0);
	isLoaded = 0;
	memShift = 0;
	//char *temp;
	CPU_p cpu = malloc(sizeof(CPU_s));
	    cpu->r[0] = 0x4100;
	    cpu->r[1] = 0x0001;
	    cpu->r[2] = 0x0001;
	    cpu->r[3] = 0x102A;
	    cpu->r[4] = 0x94CD;
	    cpu->r[5] = 0x0002;
	    cpu->r[6] = 0x9001;
	    cpu->r[7] = 0x3100;
	    cpu->ir = 0x0000;
	    cpu->PC = 0x3000;
	    cpu->MAR = 0x0000;
	    cpu->MDR = 0x0000;
	    cpu->A = 0x0000;
	    cpu->B = 0x0000;
	    cpu->N = 0;
	    cpu->Z = 0;
	    cpu->P = 0;

	    displayScreen(cpu, memShift);
	    dialog(cpu);
//	for(i = 1; i < argc; i++){
//	 	CPU_p c = malloc(sizeof(CPU_s));
//		memory[0] = strtol(argv[i], &temp, 16);
//		memory[4] = 0xA0A0;
//		controller(c);
//	}
	//  for(i = 1; i < argc; i++){
	//  	memory[i-1] = strtol(argv[i], &temp, 16);
	// }
	//  controller(c);
	return 0;
}
