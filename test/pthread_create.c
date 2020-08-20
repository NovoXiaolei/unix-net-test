#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thrd_func(void * arg){
    printf("In thread: thread id = %lu, pid = %u\n", pthread_self(), getpid());
    return NULL;
}

int
main(void){
    pthread_t tid;
    int ret;

    printf("In main 1: thread id = %lu, pid = %u\n", pthread_self(), getpid());
    ret = pthread_create(&tid, NULL, thrd_func, NULL);

    if (ret != 0){
        printf("pthread_create error, %s\n", strerror(ret));
        exit(1);
    }

    printf("In main 2: thread id = %lu, pid = %u\n", pthread_self(), getpid());
    pthread_exit(NULL);
}
