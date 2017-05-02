/*
 * Authors: Connor Lundberg, Daniel Ivanov
 * Date: 4/28/2017
 */
#include "slc3.h"

int controller (CPU_p, int);

int displayScreen (CPU_p, int);

int dialog (CPU_p cpu);

char getch ();

void setFlags (CPU_p, unsigned int, unsigned int, unsigned int);


// you can define a simple memory module here for this program
unsigned short memory[MAX_MEMORY];   // 500 words of memory enough to store simple program
int isLoaded;
int memShift;


int sext6(int offset6) {
	if (HIGH_ORDER_BIT_VALUE6 & offset6) return (offset6 | SEXT6_SIGN_EXTEND);
	else return offset6 & SEXT6_ZERO_EXTEND;
}


int sext9(int offset9) {
	if (HIGH_ORDER_BIT_VALUE9 & offset9) return (offset9 | SEXT9_SIGN_EXTEND);
	else return offset9;
}


// This is the trap function that handles trap vectors. Acts as 
// the trap vector table for now. Currently exits the HALT trap command.
int trap(CPU_p cpu, int trap_vector) {
	int value = 0;
	int i = 0;
	char *temp;
	switch (trap_vector) {
		case GETC:
			value = (int) getch();
			break;
		case OUT:
			printf("%c", cpu->gotC);
			break;
		case PUTS:
			i = 0;
			temp = memory[(cpu->r[0] - CONVERT_TO_DECIMAL + i)];
			while ((temp)) {  
			  printf("%c", (temp));
			  i++;
			  temp = memory[(cpu->r[0] - CONVERT_TO_DECIMAL + i)];
			}
			break;
		case HALT:
			value = 1;
			break;
	}
	
	return value;
}


void chooseFlag (CPU_p cpu, int cc) {
	if (cc < 0){
		setFlags(cpu, 1, 0, 0);
	}
	if (cc == 0){
		setFlags(cpu, 0, 1, 0);
	}
	if (cc > 0){
		setFlags(cpu, 0, 0, 1);
	}
}
	

void setFlags (CPU_p cpu, unsigned int neg, unsigned int zero, unsigned int pos) {
	cpu->N = neg;
	cpu->Z = zero;
	cpu->P = pos;
}


char getch() {
	char buf = 0;         
	struct termios old = {0};         
	if (tcgetattr(0, &old) < 0)                 
		perror("tcsetattr()");         
	old.c_lflag &= ~ICANON;         
	old.c_lflag &= ~ECHO;         
	old.c_cc[VMIN] = 1;         
	old.c_cc[VTIME] = 0;         
	if (tcsetattr(0, TCSANOW, &old) < 0)                 
		perror("tcsetattr ICANON");        
	if (read(0, &buf, 1) < 0)                 
		perror ("read()");         
	old.c_lflag |= ICANON;         
	old.c_lflag |= ECHO;         
	if (tcsetattr(0, TCSADRAIN, &old) < 0)                 
		perror ("tcsetattr ~ICANON");         
	return (buf); 
}


int displayScreen(CPU_p cpu, int mem) {
	printf("\t\tWelcome to the LC-3 Simulator Simulator\n\n");
	printf("\t\tRegisters \t\t    Memory\n");

	int i = TOP_HALF + mem;
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 0, cpu->r[0], i, memory[1 + mem]);

	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 1, cpu->r[1], i+1, memory[2 + mem]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 2, cpu->r[2], i+2, memory[3 + mem]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 3, cpu->r[3], i+3, memory[4 + mem]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 4, cpu->r[4], i+4, memory[5 + mem]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 5, cpu->r[5], i+5, memory[6 + mem]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 6, cpu->r[6], i+6, memory[7 + mem]);
	printf("\t\tR%d: x%04X \t\t x%X: x%04X\n", 7, cpu->r[7], i+7, memory[8 + mem]);

	i = BOTTOM_HALF + mem; // replace i with the mem dump number if you want.
	printf("\t\t\t\t\t x%X: x%04X\n",i, memory[9 + mem]);
	printf("\t\t\t\t\t x%X: x%04X\n",i+1, memory[10 + mem]);
	printf("\t\t\t\t\t x%X: x%04X\n",i+2, memory[11 + mem]);
	printf("\t\tPC:x%0.4X    IR:x%04X     x%X: x%04X\n",cpu->PC,cpu->ir,i+3, memory[12 + mem]);
	printf("\t\tA: x%04X    B: x%04X     x%X: x%04X\n",cpu->A,cpu->B,i+4, memory[13 + mem]);
	printf("\t\tMAR:x%04X  MDR:x%04X     x%X: x%04X\n",cpu->MAR + CONVERT_TO_DECIMAL,cpu->MDR,i+5, memory[14 + mem]);
	printf("\t\tCC: N: %d  Z: %01d P: %d      x%X: x%04X\n",cpu->N,cpu->Z,cpu->P,i+6, memory[15 + mem]);
	printf("\t\t\t\t\t x%X: x%04X\n",i+7, memory[16 + mem]);
	printf("  Select: 1)Load, 3)Step, 5)Display Mem, 7)Run, 9)Exit\n");
	return 0;
}


int dialog(CPU_p cpu) {
	int opNum = 0, isRunning = 0;
	char fileName[MAX_FILE_NAME];
	FILE* inputFile;
		while (opNum != EXIT) {
			scanf("%d", &opNum);
			switch (opNum) {
				case LOAD:
					printf("File Name: ");
					scanf("%s", &fileName);
					inputFile = fopen(fileName, "r");
					if (inputFile == NULL) {
						printf("DIDN'T OPEN");
						break;
					}
					int i = 0;
					while (fscanf(inputFile, "%04X", &memory[i]) != EOF) {
						if (i == 0) {
							cpu->PC = memory[0];
						}
						i++;
					}
					isLoaded = 1;
					displayScreen(cpu, 0);
					fclose(inputFile);
					break;
				case STEP:
					if (isLoaded == 1) {
						controller(cpu, 0);
						opNum = 0;
					} else {
						printf("No file loaded!");
					}
					break;
				case DISP_MEM:
					printf("Position to move to? (in decimal): ");

					scanf("%d", &memShift);
					if(memShift > MAX_MEMORY - 17) {
						printf("Error: out of memory");
						memShift = 0;
						break;
					} else {
						displayScreen(cpu, memShift);
					}
					break;
				case RUN:
					controller(cpu, 1);
					displayScreen(cpu, 0);
					break;
				case EXIT:
					printf("Simulation Terminated.");
					break;
			}
		}
}




int controller (CPU_p cpu, int isRunning) {
	unsigned int state;
	short cc;
	unsigned int opcode, Rd, Rs1, Rs2, immed_offset, BaseR;	// fields for the IR
	char charToPrint = ' ';
	char *temp;
	int value = 0;
    state = FETCH;
	int j;
    for (;;) {
        switch (state) {
            case FETCH: // microstates 18, 33, 35 in the book
            	 cpu->MAR = (cpu->PC - CONVERT_TO_DECIMAL);
            	 cpu->PC++;	// increment PC

            	                //microstate 33
            	 cpu->MDR = memory[cpu->MAR];

            	                //microstate 35
            	 cpu->ir = cpu->MDR;
            	 cc = 0;
               state = DECODE;
            case DECODE:
						// microstate 32
				opcode = cpu->ir >> SHIFT_OPCODE;
				
				Rd = cpu->ir & RETRIEVE_RD;
				Rd = (short)Rd >> SHIFT_RD;

				Rs1 = cpu->ir & RETRIEVE_RS1;
				Rs1 = (short)Rs1 >> SHIFT_RS1;

				Rs2 = cpu->ir & RETRIEVE_RS2;

				immed_offset = cpu->ir & RETRIEVE_RS1;
				
				BaseR = (cpu->ir & RETRIEVE_BASE) >> SHIFT_RS1;

                // make sure opcode is in integer form
				// hint: use four unsigned int variables, opcode, Rd, Rs, and immed7
				// extract the bit fields from the IR into these variables
                state = EVAL_ADDR;
            case EVAL_ADDR: // Look at the LD instruction to see microstate 2 example
                switch (opcode) {
					case LDR:
						cpu->MAR = (cpu->r[BaseR] + sext6(immed_offset)) - CONVERT_TO_DECIMAL;
						break;
					case LD:
						cpu->MAR = (cpu->PC - CONVERT_TO_DECIMAL) + sext9(immed_offset);
						break;
					case ST:
						cpu->MAR = (cpu->PC - CONVERT_TO_DECIMAL) + sext9(immed_offset);
						break;
					case STR:
						cpu->MAR = (cpu->r[BaseR] - CONVERT_TO_DECIMAL) + sext6(immed_offset);
						break;
					case TRAP:
						cpu->MAR = immed_offset & RETRIEVE_TRAP_VECTOR;
						break;
                // different opcodes require different handling
                // compute effective address, e.g. add sext(immed7) to register
                }
                state = FETCH_OP;
            case FETCH_OP: // Look at ST. Microstate 23 example of getting a value out of a register
                switch (opcode) {
					case LDR:
					case LD:
					 cpu->MDR = memory[cpu->MAR];
						break;
					case ADD:
						if(HIGH_ORDER_BIT_VALUE6 & cpu->ir){ //0000|0000|0010|0000
							cpu->A = cpu->r[Rs1];
							cpu->B = (immed_offset & IMMED_OFFSET_MASK);
						} else{
							cpu->A = cpu->r[Rs1];
							cpu->B = cpu->r[Rs2];
						}

						break;
					case AND:
					if(HIGH_ORDER_BIT_VALUE6 & cpu->ir){ //0000|0000|0010|0000
							cpu->A = cpu->r[Rs1];
							cpu->B = (immed_offset & IMMED_OFFSET_MASK);
						} else{
							cpu->A = cpu->r[Rs1];
							cpu->B = cpu->r[Rs2];
						}
						break;
					case NOT:
						cpu->A = cpu->r[Rs1];
						break;
					case STR:
					case ST:
						cpu->MDR = cpu->r[Rd];
						break;
					case TRAP:
						cpu->MDR = memory[cpu->MAR];
						cpu->r[7] = cpu->PC;
                }
                state = EXECUTE;
            case EXECUTE: // Note that ST does not have an execute microstate
                switch (opcode) {
                    // do what the opcode is for, e.g. ADD
                    // in case of TRAP: call trap(int trap_vector) routine, see below for TRAP x25 (HALT)
					case ADD:
						// checks for negative addition
						if (cpu->A < 0) {
							cpu->Res = -(cpu->A) + (cpu->B);
						} else if (cpu->B < 0) {
							cpu->Res = (cpu->A) -(cpu->B);
						} else if ((cpu->A < 0) & (cpu->B < 0)) {
							cpu->Res = -(cpu->A) -(cpu->B);
						} else {
							cpu->Res = (cpu->A) + (cpu->B);
						}
						cc = (short int) cpu->Res;
						chooseFlag (cpu, cc);
						break;
					case AND:
						cpu->Res = cpu->A & cpu->B;
						cpu->N = 0;
						cpu->Z = 0;
						cpu->P = 0;
						cc = cpu->Res;
						chooseFlag (cpu, cc);
						break;
					case NOT:
						cpu->Res = ~(cpu->A);
						cpu->N = 0;
						cpu->Z = 0;
						cpu->P = 0;
						cc = (short) cpu->Res;
						chooseFlag (cpu, cc);
						break;
					case TRAP:
						switch (cpu->MAR) {
							case PUTS:
								cpu->out = memory[(cpu->r[0] - CONVERT_TO_DECIMAL)];
								break;
						}
						cpu->PC = cpu->MDR;
						value = trap(cpu, cpu->MAR);
						cpu->PC = cpu->r[7];
						
						if (value == 1) {
							return 0;
						} else if (value > 1) {
							cpu->r[0] = (char) value;
							cpu->gotC = (char) value;
							cpu->r[Rd] = value;
						}
						break;
					case JSRR:
						cpu->r[7] = cpu->PC;
						cpu->PC = cpu->r[BaseR];
						break;
					case JMP:
						cpu->r[7] = cpu->PC;
						cpu->PC = cpu->r[Rs1];
						break;
					case BR:
						if (cpu->N && (Rd & N_BIT)) {
							cpu->PC = cpu->PC + sext9(immed_offset);
							break;
						}
						if (cpu->Z && (Rd & Z_BIT)) {
							cpu->PC = cpu->PC + sext9(immed_offset);
							break;
						}
						if (cpu->P && (Rd & P_BIT)) {
							cpu->PC = cpu->PC + sext9(immed_offset);
							break;
						}
					break;
                }
                state = STORE;
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
					case LDR:
					case LD:
						cpu->r[Rd] = cpu->MDR;
						cc = cpu->r[Rd];
						chooseFlag (cpu, cc);
						break;
					case LEA:
						cpu->r[Rd] = cpu->PC + sext9(immed_offset);
						cc = cpu->r[Rd];
						chooseFlag (cpu, cc);
						break;
					case STR:
					case ST:
						memory[cpu->MAR] = cpu->MDR;
						break;
						
                    // write back to register or store MDR into memory
                }
                // do any clean up here in prep for the next complete cycle
                state = FETCH;
                break;
        }
		if (!isRunning) {
			displayScreen(cpu, 0);
			scanf("%c", &charToPrint);
		}
    }
}


int main(int argc, char* argv[]){

	setvbuf(stdout, NULL, _IONBF, 0);
	isLoaded = 0;
	memShift = 0;
	CPU_p cpu = malloc(sizeof(CPU_s));
	for (int i = 0; i < REG_FILE_SIZE; i++) {
		cpu->r[i] = 0x0000;
	}
	cpu->ir = 0x0000;
	cpu->PC = TOP_HALF;
	cpu->MAR = 0x0000;
	cpu->MDR = 0x0000;
	cpu->A = 0x0000;
	cpu->B = 0x0000;
	cpu->N = 0;
	cpu->Z = 0;
	cpu->P = 0;
		
	displayScreen(cpu, memShift);
	dialog(cpu);
	return 0;
}
