#include <stdlib.h>
#include <pthread.h>
#include "hashmap.h"

typedef struct bucketNode {
    item kv; //store the pair for each value in the bucket
    struct bucketNode *next; //point to the next node in bucket
} bucketNode;

typedef struct {
    bucketNode *head;
    pthread_mutex_t lock; //give a lock for each bucket
} bucket;

int mapSize;
bucket *buckets;

int init(int size) {
    mapSize = size;

    buckets = malloc(size * sizeof(bucket)); //allocate space for the buckets
    for (int i = 0; i < size; i++) {
        buckets[i].head = NULL;
        pthread_mutex_init(&buckets[i].lock, NULL);
    }

    return 0;
}

void put(item kv_item) {

    int hashVal = kv_item.key % mapSize;
    pthread_mutex_lock(&buckets[hashVal].lock); //lock the bucket for the key

    bucketNode *current = buckets[hashVal].head;
    while (current != NULL) {
        if (current->kv.key == kv_item.key) {
            current->kv.value = kv_item.value;
            pthread_mutex_unlock(&buckets[hashVal].lock);
            return;
        }
        current = current->next;
    }

    bucketNode *newNode = malloc(sizeof(bucketNode));
    newNode->kv = kv_item;
    newNode->next = NULL;

    newNode->next = buckets[hashVal].head;
    buckets[hashVal].head = newNode;

    pthread_mutex_unlock(&buckets[hashVal].lock); //unlock the bucket for the key
    return;
}

item* get(int key) {

    int hashVal = key % mapSize;
    pthread_mutex_lock(&buckets[hashVal].lock); //lock the bucket for the key

    bucketNode *current = buckets[hashVal].head;

    while (current != NULL) {
        if (current->kv.key == key) {
            pthread_mutex_unlock(&buckets[hashVal].lock);
            return &current->kv;
        }
        current = current->next;
    }

    pthread_mutex_unlock(&buckets[hashVal].lock); //unlock the bucket for the key
    return NULL;
}