#include <stdio.h>
#include <libthrd.h>

void test(void *params){
    int val = *((int *) params);
    printf("test :%d:", val);
}

int main(void){
    int val = 156;
    lanceThread(test, (void *) (&val), 1);
    printf("fin");
    return 0;
}
