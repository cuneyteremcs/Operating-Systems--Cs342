

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

FILE *temp_file[10][50];
FILE *temp_file2[50];


struct thread_data {
	int R;
	int i;
	char *filename;
	int date1;
	int date2;
	int id1;
	int id2;
};

typedef struct thread_data DATA;



struct  Node {
	int callid;
	int count;
	struct Node *calledIDList;
	struct Node *next; 
};

typedef struct Node NODE;


NODE *sortList(NODE *list) {
	if(list == NULL || list->next == NULL)
		return list;

	NODE *cur, *minimum;
	cur = list;
	minimum = list;

	while(cur != NULL) {
		if(cur->callid < minimum->callid)
			minimum = cur;
		cur = cur->next;
	}

	int temp = list->callid;
	int temp3 = list->count;
	NODE *temp2=list->calledIDList;
	list->callid = minimum->callid;
	list->count = minimum->count;
	list->calledIDList=minimum->calledIDList;
	minimum->callid = temp;
	minimum->count = temp3;
	minimum->calledIDList=temp2;

	list->next = sortList(list->next);
	return list;
}

void *func_mapper(void *data) {
	DATA *d = data;

	printf("\n filename: %s , index: %d , R: %d \n", d->filename, d->i, d->R);

	FILE *file = fopen(d->filename, "r");

	char line[24];

	int i;
	for(i = 0; i < d->R; i++) 
		temp_file[d->i][i] = tmpfile();

	while(fgets(line, sizeof(line), file)) {
		char IDcaller[9];

		strncpy(IDcaller, line, 8);
		IDcaller[9] = '\0';

		int num = atoi(IDcaller);
		

		fprintf(temp_file[d->i][num%(d->R)], "%s", line);

		//printf(" %d \n", num);
	}

	for(i = 0; i < d->R; i++) 
		rewind(temp_file[d->i][i]);

	fclose(file);
	pthread_exit(NULL);
	return NULL;

}

void *func_reduce(void *data) {
	DATA *d = data;
	NODE *list = NULL;

	//printf("\n index: %d , N: %d \n", d->i, d->R);

	temp_file2[d->i] = tmpfile();

	char line[24];
	int i;
	for(i = 0; i < d->R; i++) {
		while(fgets(line, sizeof(line), temp_file[i][d->i])) {
			//printf( " \n reduce temporary file : %s , N: %d, index: %d \n" ,line, i, d->i);
			
			char IDcaller[9];
			char date[5];
			char IDcalled[9];

			strncpy(IDcaller, line, 8);
			IDcaller[9] = '\0';

			int num = atoi(IDcaller);

			strncpy(IDcalled, line+9, 8);
			IDcalled[9] = '\0';

			int called = atoi(IDcalled);

			strncpy(date, line+18, 4);
			date[5] = '\0';

			int num2 = atoi(date);

			int exist=0;
			int exist2=0;
			if(num2 <= (d->date2) && num2 >= (d->date1) && num <= (d->id2) && num >= (d->id1)) {
				if(list==NULL) {
					list = (NODE*)malloc(sizeof(NODE));
					list->next = NULL;
					list->callid=num;
					list->calledIDList=(NODE*)malloc(sizeof(NODE));
					list->calledIDList->callid=called;
					list->calledIDList->next=NULL;
					//list->count=1;
				}

				else {

					NODE *cur;
					

					for(cur = list; cur != NULL; cur = cur->next) {

						if(cur->callid == num) {
							//cur->count += 1;		
							NODE *temp;
							for(temp=cur->calledIDList;temp!=NULL;temp=temp->next){
								if(temp->callid == called) {
									exist2=1;
								}

							}

							if(exist2==0){
								NODE *new;
								new = (NODE*)malloc(sizeof(NODE));
								new->callid = called;
								new->next = cur->calledIDList->next;
								cur->calledIDList->next = new;
							}
							exist=1;				
						}
						//printf("\n id: %d count: %d \n", cur->callid, cur->count);
					}

					if(exist ==0) {
						NODE *current;
						current = (NODE*)malloc(sizeof(NODE));
						current->callid = num;
						//current->count = 1;
						current->calledIDList=(NODE*)malloc(sizeof(NODE));
						current->calledIDList->callid=called;
						current->calledIDList->next=NULL;
						current->next = list->next;
						list->next = current;

					}
				}
			}
			exist=0;
			exist2=0;
			//printf(" %d , %d \n", num, num2);
		}
	}

	sortList(list);

	NODE *cur;

	for(cur = list; cur != NULL; cur = cur->next)  {
		int occurence=0;
		NODE *cur2;
		//printf("for called id %d calls are: ", cur->callid);
		for(cur2 = cur->calledIDList; cur2 != NULL; cur2 = cur2->next){
			occurence++;
			//printf("	%d: ", cur2->callid);
		} 
		//printf("%d \n", occurence);
		
		fprintf(temp_file2[d->i], "%d ", cur->callid);
		fprintf(temp_file2[d->i], "%d", occurence);
		fprintf(temp_file2[d->i], "\n");
	}
	
	rewind(temp_file2[d->i]);

	pthread_exit(NULL);
	return NULL;
}

void *func_merger(void *data) {

	DATA *d = data;
	NODE *list = NULL;

	int i;
	char line[15];

	for(i = 0; i < d->R; i++) {
		while(fgets(line, sizeof(line), temp_file2[i])) {
			char num1[9];
			char num2[3];

			strncpy(num1, line, 8);
			num1[9] = '\0';

			strncpy(num2, line+9, 3);
			num2[3] = '\0';

			int num = atoi(num1);
			int number2 = atoi(num2);


			//printf(" %d , %d \n", num, number2);
			if(list==NULL) {
					list = (NODE*)malloc(sizeof(NODE));
					list->next = NULL;
					list->callid=num;
					list->count=number2;
				}

				else {
						NODE *current;
						current = (NODE*)malloc(sizeof(NODE));
						current->callid = num;
						current->count = number2;
						current->next = list->next;
						list->next = current;
				}

		}
	}

	sortList(list);
	

	FILE *file = fopen(d->filename, "w");

	NODE *cur;

	for(cur = list; cur != NULL; cur = cur->next) {
		fprintf(file, "%d ", cur->callid);
		fprintf(file, " %d \n", cur->count);
	}

	fclose(file);
	pthread_exit(NULL);
	return NULL;

}


int main(int argc, char *argv[]) {

	clock_t start = clock();

	int N, R, date1, date2, id1, id2;
	char line[24];

	sscanf (argv[1], "%i", &N);
	sscanf (argv[2], "%i", &R);

	sscanf (argv[4+N], "%i", &date1);
	sscanf (argv[5+N], "%i", &date2);
	sscanf (argv[6+N], "%i", &id1);
	sscanf (argv[7+N], "%i", &id2);


	pthread_t thread_mapper[N];
	int t, i, j;
	DATA data[N];

	for(i = 0; i < N; i++) {

		data[i].R = R;
		data[i].i = i;
		data[i].filename = argv[i+3];
		data[i].date1 = date1;
		data[i].date2 = date2;
		data[i].id1 = id1;
		data[i].id2 = id2;


		t = pthread_create(&thread_mapper[i], NULL, &func_mapper, (void*)&data[i]);

		if(t != 0) {
			perror("\n thread mapper is not created \n");
			exit(1);
		}
		printf("\n thread mapper created \n");

	}

	for(i = 0; i < N; i++) {
		pthread_join(thread_mapper[i], NULL);
	}

	/*	
	for(i = 0; i < N; i++) {
		for(j = 0; j < R; j++ ) {
			while(fgets(line, sizeof(line), temp_file[i][j])) {
				printf(" \n temporary file: %d %d : %s\n", i, j, line);
			}
			rewind(temp_file[i][j]);
		}
	}
	*/
	

	DATA data2[R];

	pthread_t thread_reduce[R];
	pthread_t thread_merger;

	for(i = 0; i < R; i++) {
		data2[i].R = N;
		data2[i].i = i;
		data2[i].filename = argv[i+3];
		data2[i].date1 = date1;
		data2[i].date2 = date2;
		data2[i].id1 = id1;
		data2[i].id2 = id2;

		t = pthread_create(&thread_reduce[i], NULL, &func_reduce, (void*)&data2[i]);

		if(t != 0) {
			perror("\n thread reduder is not created \n");
			exit(1);
		}
		printf("\n thread reducer created \n");

	}

	for(i = 0; i < R; i++) {
			pthread_join(thread_reduce[i], NULL);
	}


	DATA data3;
	data3.R = R;
	data3.i = N;
	data3.filename = argv[N+3];
	data3.date1 = date1;
	data3.date2 = date2;
	data3.id1 = id1;
	data3.id2 = id2;

	t = pthread_create(&thread_merger, NULL, &func_merger, (void*)&data3);

	if(t != 0) {
		perror("\n thread merger is not created \n");
		exit(1);
	}
	printf("\n thread merger created \n");

	pthread_join(thread_merger, NULL);

	for(i = 0; i < N; i++) {
		for(j = 0; j < R; j++ ) {
		fclose(temp_file[i][j]);
		}
	}

	for(j = 0; j < R; j++ ) {
		fclose(temp_file2[j]);
	}

	clock_t end = clock();

	double time = (double) (end - start) / CLOCKS_PER_SEC;

	printf("\n %f \n", time);
	
	return 0;
}