#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include "hash.h"

HashTable *ht1; // space allocated inside library

typedef struct PARAMETER {
    int tid;
    int nInsThreads;
    int N;
} Parameter;

void * insertThread(void * ptr) {

    Parameter * parameter = (Parameter *) ptr;

    int i = 0;
    int l=5;
    for (i = parameter->tid; i < parameter->N; i += parameter->nInsThreads) {
        printf("Pthread %d calling hashInsert with key-value: %d-%d\n", parameter->tid, i*10000, parameter->tid );
        l=hash_insert(ht1, i*10000, parameter->tid);
        printf("Return from hash is: %d\n", l);
    }

    pthread_exit(NULL);
}

void * deleteThread(void * ptr) {

    Parameter * parameter = (Parameter *) ptr;

    int i = 0;

    for (i = 2; i < parameter->N; i += parameter->nInsThreads) {

        printf("Pthread %d calling hash_delete with key: %d\n", parameter->tid, i*10000);
        while (hash_delete(ht1, i*10000) < 0);
    }

    pthread_exit(NULL);
}

int main() {

    clock_t begin = clock();
    int i;

    int N = 12600, M = 126;
    
    ht1 = hash_init(N, M);

    int nthreads = 5;
    int iret;

    Parameter parameter[nthreads];
    pthread_t threads[nthreads];

    //thread parameters
    for (int i = 0; i < nthreads; i++) {
        parameter[i].tid = i;
        parameter[i].nInsThreads = nthreads - 1;
        parameter[i].N = N;
    }

    for (i = 0; i < 4; i++) {
        iret = pthread_create(&threads[i], NULL, insertThread, &parameter[i]);
        if (iret) {
            fprintf(stderr, "Error - pthread_create() return code: %d\n", iret);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 4; i < 5; i++) {
        iret = pthread_create(&threads[i], NULL, deleteThread, &parameter[i]);
        if (iret) {
            fprintf(stderr, "Error - pthread_create() return code: %d\n", iret);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
    }

    int error = 0;

    //Check results
    for (i = 0; i < N; i += (nthreads - 1)) {
        int val;
        printf("hash_get with key: %d\n", i*10000);
        if (hash_get(ht1, i*10000, &val) == 0) {

            printf("The value is : %d\n", val );
            if (val != 0)
                error = 1;
        }
        else
            error = 1;

    }
    
    for (i = 1; i < N; i += (nthreads - 1)) {
        int val;
        printf("hash_get with key: %d\n", i*10000);
        if (hash_get(ht1, i*10000, &val) == 0) {
            printf("The value is : %d\n", val );
            if (val != 1)
                error = 1;
        }
        else
            error = 1;
    }
    
    for (i = 3; i < N; i += (nthreads - 1)) {
        int val;
        printf("hash_get with key: %d\n", i*10000);
        if (hash_get(ht1, i*10000, &val) == 0) {
            printf("The value is : %d\n", val );
            if (val != 3)
                error = 1;
        }
        else
            error = 1;
    }
    
    for (i = 2; i < N; i += (nthreads - 1)) {
        int val;
        printf("hash_get with key: %d\n", i*10000);

        if (hash_get(ht1, i*10000, &val) == 0) {
            printf("The value is : %d\n", val );
            error = 1;
        }
    }
    
    hash_destroy(ht1);

    switch (error) {
        case 0:printf("OK!\n");
            break;
        case 1:printf("NOT OK!\n");
    }


    clock_t end = clock();
 double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Program executed in %fs\n", time_spent);
}
