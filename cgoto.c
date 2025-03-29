#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

// https://eli.thegreenplace.net/2012/07/12/computed-goto-for-efficient-dispatch-tables

#define OP_HALT     0x0
#define OP_INC      0x1
#define OP_DEC      0x2
#define OP_MUL2     0x3
#define OP_DIV2     0x4
#define OP_ADD7     0x5
#define OP_NEG      0x6

int interp_switch(unsigned char* code, int initval)
{
    printf("\n\tSwitch...\n\n");
    
    int pc = 0;
    int val = initval;

    while (1) {
        switch (code[pc++]) {
            case OP_HALT:
                return val;
            case OP_INC:
                val++;
                break;
            case OP_DEC:
                val--;
                break;
            case OP_MUL2:
                val *= 2;
                break;
            case OP_DIV2:
                val /= 2;
                break;
            case OP_ADD7:
                val += 7;
                break;
            case OP_NEG:
                val = -val;
                break;
            default:
                return val;
        }
    }
}

int interp_cgoto(unsigned char* code, int initval)
{
    printf("\n\tComputed goto...\n\n");
    
    static void* dispatch_table[] = {
        &&do_halt, &&do_inc, &&do_dec, &&do_mul2,
        &&do_div2, &&do_add7, &&do_neg};
    #define DISPATCH() goto *dispatch_table[code[pc++]]

    int pc = 0;
    int val = initval;

    DISPATCH();
    while (1) {
        do_halt:
            return val;
        do_inc:
            val++;
            DISPATCH();
        do_dec:
            val--;
            DISPATCH();
        do_mul2:
            val *= 2;
            DISPATCH();
        do_div2:
            val /= 2;
            DISPATCH();
        do_add7:
            val += 7;
            DISPATCH();
        do_neg:
            val = -val;
            DISPATCH();
    }
}

int main()
{
    unsigned char code[] =  { OP_HALT ,
                              OP_INC ,
                              OP_DEC ,
                              OP_MUL2 ,
                              OP_DIV2 ,
                              OP_ADD7 ,
                              OP_NEG };

    int ret = interp_switch(code, 42);
    printf("Result from Switch: %d\n", ret);
    
    ret = interp_cgoto(code, 42);
    printf("Result from CGoto: %d\n", ret);
    
	return 0;
}
