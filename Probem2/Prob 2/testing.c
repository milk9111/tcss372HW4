#include<stdio.h>

int main()
{
    unsigned short ir = 21664;

    unsigned int opcode, Rd, Rs1, Rs2, immed_offset;

    opcode = ir >> 12;

    Rd = ir & 0x0fff;
    Rd = (short)Rd >> 9;

    Rs1 = ir & 0x01ff;
    Rs1 = (short)Rs1 >> 6;

    Rs2 = ir & 0x0007;

    immed_offset = ir & 0x01ff;

    printf("%d\n%d\n%d\n%d\n%d\n", opcode, Rd, Rs1, Rs2, immed_offset);

    //unsigned short Rd
    //printf("hello\n");
    return 0;
}
