#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "hashmap.h"

void *worker(void *arg) {
    int *id = (int *)arg;
    int start = (*id) * 100;
    int end = start + 100;
    for (int i = start; i < end; i++) {
        item addItem;
        addItem.key = i;
        addItem.value = i * 2;
        put(addItem);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    init(1000);

    printf("\n---------- BASE TEST ----------\n");

    int inKey = random() % 1000;
    int inVal = random() % 1000;

    item addItem;
    addItem.key = inKey;
    addItem.value = inVal;

    put(addItem);

    item *getItem = get(inKey);

    if (getItem == NULL) {
        printf("ERROR\nget returned NULL unexpectedly.\n");
    } else if (getItem->value != inVal) {
        printf("ERROR\nVALUES DID NOT MATCH.\nINPUT:  %d\nOUTPUT: %d\n", inVal, getItem->value);
    } else {
        printf("Values matched as expected.\n");
    }

    printf("\n-------- UPDATING TEST --------\n");

    addItem.value = inVal + 5;
    put(addItem);

    getItem = get(inKey);

    if (getItem == NULL) {
        printf("ERROR\nget returned NULL unexpectedly.\n");
    } else if (getItem->value != inVal + 5) {
        printf("ERROR\nVALUES DID NOT MATCH.\nINPUT:  %d\nOUTPUT: %d\n", inVal+5, getItem->value);
    } else {
        printf("Values matched as expected.\n");
    }

    printf("\n-------- EMPTY KEY TEST -------\n");

    getItem = get(inKey + 5);

    if (getItem != NULL) {
        printf("ERROR\nget did not NULL unexpectedly.\n");
    } else {
        printf("NULL returned as expected.\n");
    }

    printf("\n------ MULTI THREAD TEST ------\n");

    int numThreads = 4;
    pthread_t threads[numThreads];
    int ids[numThreads];

    for (int i = 0; i < numThreads; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    int passed = 1;
    for (int i = 0; i < numThreads; i++) {
        for (int j = 0; j < 100; j++) {
            int expectedKey = i * 100 + j;
            item *result = get(expectedKey);
            if (result == NULL || result->key != expectedKey) {
                printf("ERROR: key %d not found or mismatched\n", expectedKey);
                passed = 0;
            }
        }
    }

    if (passed) {
        printf("All keys found as expected.\n");
    }

}