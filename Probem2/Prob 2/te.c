// lc3.h

#define FETCH 0
#define DECODE 1
#define EVAL_ADDR 2

// etc.
/*typedef unsigned short Register;
cpu {
  Register reg_file[size];
  Register PC, IR;
}*/


int main() {


  return 0;
}

// lc3.c

// you can define a simple memory module here for this program
unsigned short memory[32];   // 32 words of memory enough to store simple program

int controller (CPU_p cpu) {
    // check to make sure both pointers are not NULL
    // do any initializations here
	unsigned int opcode, Rd, Rs1, Rs2, immed_offset;	// fields for the IR
    state = FETCH;
    for (;;) {   // efficient endless loop
        switch (state) {
            case FETCH: // microstates 18, 33, 35 in the book
                printf("Here in FETCH\n")
                // get memory[PC] into IR - memory is a global array
                // increment PC
                cpu->ir = memory[PC];
                cpu->PC++;
                printf("Contents of IR = %04X\n", cpu->ir);

               state = DECODE;
                break;
            case DECODE: // microstate 32
                opcode = cpu->ir >> 12;

                Rd = cpu->ir & 0x0fff;
                Rd = (short)Rd >> 9;

                Rs1 = cpu->ir & 0x01ff;
                Rs1 = (short)Rs1 >> 6;

                Rs2 = cpu->ir & 0x0007;

                immed_offset = cpu->ir & 0x01ff;

                // make sure opcode is in integer form
				// extract the bit fields from the IR into these variables
                state = EVAL_ADDR;
                break;
            case EVAL_ADDR: // Look at the LD instruction to see microstate 2 example
                switch (opcode) {
                // different opcodes require different handling
                // compute effective address, e.g. add sext(immed7) to register
                }
                state = FETCH_OP;
                break;
            case FETCH_OP: // Look at ST. Microstate 23 example of getting a value out of a register
                switch (opcode) {
                    // get operands out of registers into A, B of ALU
                    // or get memory for load instr.
                }
                state = EXECUTE;
                break;
            case EXECUTE: // Note that ST does not have an execute microstate
                switch (opcode) {
                    // do what the opcode is for, e.g. ADD
                    // in case of TRAP: call trap(int trap_vector) routine, see below for TRAP x25 (HALT)
                }
                state = STORE;
                break;
            case STORE: // Look at ST. Microstate 16 is the store to memory
                switch (opcode) {
                    // write back to register or store MDR into memory
                }
                // do any clean up here in prep for the next complete cycle
                state = FETCH;
                break;
        }
    }
}
