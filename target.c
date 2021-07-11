#include <stdio.h>
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;
typedef float          float32;
typedef double         float64;


typedef union UN_A_T{
    uint8 i;
    uint8 j;
    uint8 k;
}UN_A;



typedef struct ST_A_T{
    UN_A un_a1;
    UN_A un_a2;
    struct {
        uint32 test1;
        uint32 test2;
    };

    float32 f;
    float64 d;
}ST_A;

typedef struct ST_B_T{
    ST_A a1;
    ST_A f;
    ST_A d;
}ST_B;

static ST_B static_global_b;
ST_B global_b;

int main(void){
    
    static ST_B static_local_b;
    ST_B local_b;
    
    printf("static_global_b : %p\n",&static_global_b);
    printf("       global_b : %p\n",&global_b);
    printf("static_local_b  : %p\n",&static_local_b);
    printf("       local_b  : %p\n",&local_b);

    return 0;
}
