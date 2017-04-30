
#include <stdio.h>
#include <pthread.h>
#include "hash.h"
#include <stdlib.h>

// remove printf debug statements at the end

HashTable *hash_init (int N, int M)
{
	printf ("hash_init called\n"); 
	printf("N = %d\n",N);
	printf("M = %d\n",M);
	HashTable * hp = (HashTable*)malloc(sizeof(HashTable)); 
	hp->N = N;
	hp->M = M;
	int KRegion = N/M;
	hp->locks = (pthread_mutex_t*)malloc(KRegion*sizeof(pthread_mutex_t)); 
	int i;
	for(i = 0; i < KRegion; i++){
		pthread_mutex_init(&(hp->locks[i]) ,NULL);
	}
	hp->buckets = malloc(N*sizeof(Node*));
	for(i = 0; i < N ; i++){
		hp->buckets[i] = NULL;
	}
		
	return (hp); 
}

int hash_insert (HashTable *hp, int k, int v)
{

	printf ("\n\nhash_insert called\n"); 
	int bucketNumber = k % hp->N;
	int lockNumber = bucketNumber / hp->M;
	printf("N = %d\n",hp->N);
	printf("M = %d\n",hp->M);
	printf("Key = %d\n",k);
	printf("Value = %d\n",v);
	printf("bucketNumber = %d\n",bucketNumber);
	printf("lockNumber = %d\n",lockNumber);
	pthread_mutex_lock(&hp->locks[lockNumber]);

	if(hp->buckets[bucketNumber] == NULL){
		hp->buckets[bucketNumber] = malloc(sizeof(Node)); 
		hp->buckets[bucketNumber]->key = k;
		hp->buckets[bucketNumber]->value = v;
		hp->buckets[bucketNumber]->next = NULL;
		printf("Key %d added to head\n",k);
		pthread_mutex_unlock(&hp->locks[lockNumber]);
		return 1;
	} else{//if the head already exist
		Node *head = hp->buckets[bucketNumber];
		while(head->key != k && head->next != NULL){
			head = head->next;
		}
		if(head->key == k){
			printf("Key %d already exist\n",k );
			pthread_mutex_unlock(&hp->locks[lockNumber]);
			return -1;
		}
		head->next = malloc(sizeof(Node)); 
		head->next->key = k;
		head->next->value = v;
		head->next->next = NULL;
		printf("Key %d added to list\n",k);
		pthread_mutex_unlock(&hp->locks[lockNumber]);
		return 1;
	}


	pthread_mutex_unlock(&hp->locks[lockNumber]);
	return (0); 
}

int hash_delete (HashTable *hp, int k) {

	printf ("\n\nhash_delete called\n"); 
	int bucketNumber = k % hp->N;
	int lockNumber = bucketNumber / hp->M;
	printf("N = %d\n",hp->N);
	printf("M = %d\n",hp->M);
	printf("Key = %d\n",k);
	printf("bucketNumber = %d\n",bucketNumber);
	printf("lockNumber = %d\n",lockNumber);
	pthread_mutex_lock(&hp->locks[lockNumber]);

	Node *head = hp->buckets[bucketNumber];
	if(head == NULL){
		printf("The bucket is empty\n");
		pthread_mutex_unlock(&hp->locks[lockNumber]);
		return -1;
	} 

	if(head-> key == k){
		printf("Head %p \n",head);
		printf("Head->next %p \n",head->next);
		hp->buckets[bucketNumber] = head->next;
		free(head);
		printf("Key %d deleted from head\n",k);
		pthread_mutex_unlock(&hp->locks[lockNumber]);
		return 1;
	}

	Node *cur=head;

	while(cur->next != NULL && cur->next->key != k){
		cur = cur->next;
	}

	if(cur->next != NULL && cur->next->key == k){
		//Node *keyNode = hp->buckets[bucketNumber];
		Node *temp=cur->next;
		cur->next = cur->next->next;
		free(temp);
		//temp=NULL;
		printf("Key %d deleted from head\n",k);
		pthread_mutex_unlock(&hp->locks[lockNumber]);
		return 0;
	}
	printf("Key %d does not exist in the list\n",k);
	pthread_mutex_unlock(&hp->locks[lockNumber]);
	return -1;
}

int hash_get (HashTable *hp, int k, int *vptr)
{
	printf ("\n\nhash_get called\n");  
	int bucketNumber = k % hp->N;
	int lockNumber = bucketNumber / hp->M;
	printf("N = %d\n",hp->N);
	printf("M = %d\n",hp->M);
	printf("Key = %d\n",k);
	printf("bucketNumber = %d\n",bucketNumber);
	printf("lockNumber = %d\n",lockNumber);
	pthread_mutex_lock(&hp->locks[lockNumber]);

	Node *head = hp->buckets[bucketNumber];
	if(head == NULL){
		printf("The bucket is empty\n");
		pthread_mutex_unlock(&hp->locks[lockNumber]);
		return -1;
	} 

	while(head != NULL){
		if(head->key == k){
			printf("Key %d found value = %d\n",k,head->value);
			*vptr = head->value;
			pthread_mutex_unlock(&hp->locks[lockNumber]);
			return (0); 
		}
		head = head->next;
	}
	printf("Key %d could not found\n",k);
	pthread_mutex_unlock(&hp->locks[lockNumber]);
	return -1;
}

int hash_destroy (HashTable *hp)
{
	printf ("hash_destroy called\n"); 
	int i;
	for(i = 0; i < hp->N; i++){
		while(hp->buckets[i] != NULL){
			Node *head = hp->buckets[i];
			hp->buckets[i] = hp->buckets[i]->next;
			free(head);
		}
	}

	int KRegion = hp->N/hp->M;
	for(i = 0; i < KRegion; i++){
		pthread_mutex_destroy(&(hp->locks[i]));
	}

	free(hp->locks);
	free(hp);
	printf("destroy finished!\n");
	return (0); 
}
