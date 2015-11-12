#include "rts.h"

void process1(int iter)
{
    printf("iter:%d\n",iter);
    puts("hello process 1");
    rts_sync();   
    puts("hello process 1");
    rts_sync();   
}

void process2(int iter)
{
    printf("iter:%d\n",iter);
    puts("hello process 2");
    rts_sync();   
    printf("iter:%d\n",iter);
    puts("hello process 2");
    rts_sync();   
}

void process3(int iter)
{
    printf("iter:%d\n",iter);
    puts("hello process 3");
    rts_sync();   
    printf("iter:%d\n",iter);
    puts("hello process 3");
    rts_sync();   
}

int main(int argc, char **argv) 
{
    t_rts_procdecl proc ={process1,process2,process3};  
    rts_run(100,3,proc);
    return 0;
}
