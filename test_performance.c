#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "hashmap.h"

#define OPERATIONS 100000000

void *worker(void *arg) {
    for (int i = 0; i < OPERATIONS; i++) {
        int key = rand() % 1000000;
        int doGet = rand() % 2;
        if (doGet == 1) {
            get(key);
        } else {
            item addItem;
            addItem.key = key;
            addItem.value = rand();
            put(addItem);
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL)); //Make sure random value is actually random
    init(10000);
    int threadCount[] = {1, 2, 3, 5};
    double timers[4];
    double timePerMil[4];

    for (int i = 0; i < 4; i++) {
        int numThreads = threadCount[i];

        pthread_t threads[numThreads];

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int j = 0; j < numThreads; j++) {
            pthread_create(&threads[j], NULL, worker, NULL);
        }

        for (int j = 0; j < numThreads; j++) {
            pthread_join(threads[j], NULL);
        }

        clock_gettime(CLOCK_MONOTONIC, &end);

        timers[i] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        double totalOps = numThreads * OPERATIONS;
        timePerMil[i] = timers[i] / (totalOps / 1000000.0);
    }
    

    printf("\n----------TIMES----------\n");
    printf("Thread Count|Time Per Million Operations (s)\n");
    for(int k = 0; k < 4; k++) {
        printf("%d           |%.6f\n", threadCount[k], timePerMil[k]);
    }
}