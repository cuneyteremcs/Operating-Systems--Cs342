#ifndef HASH_H
#define HASH_H

#include <pthread.h>

#define MIN_N 100
#define MAX_N 1000
#define MIN_M 10
#define MAX_M 1000

struct hash_table
{
	pthread_mutex_t *locks;
	int N;
	int M;
	int id;
    struct hash_table *next;
	struct node **buckets;
};

struct node{
	int key;
	int value;
	struct node *next;
};

typedef struct hash_table HashTable;
typedef struct node Node;


HashTable *hash_init (int N, int M);
int hash_insert (HashTable *hp, int k, int v);
int hash_delete (HashTable *hp, int k);
int hash_get (HashTable *hp, int k, int *vptr);
int hash_destroy (HashTable *hp); 


#endif /* HASH_H */
