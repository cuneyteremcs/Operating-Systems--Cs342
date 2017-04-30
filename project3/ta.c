#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

sem_t availableChairs, TaDesk,TaSleep,TaHelps;
int continueExecution = 1;

void *taFunction() {
	while (continueExecution) {
		int availableChairNumber;
		sem_getvalue(&availableChairs, &availableChairNumber);
		int searchforHelp;
		sem_getvalue(&TaHelps, &searchforHelp);

		if(availableChairNumber == 3 && continueExecution && searchforHelp == 0){
			printf("TA takes a nap\n");
			sem_wait(&TaSleep);
		}

		sleep(1);

		if (continueExecution) {
	    	sleep(1);
		    sem_post(&TaHelps);
	    	printf("TA checks chair\n");
	    	sleep(1);
		}
	}
	pthread_exit(NULL);
}

void *studentFunction(void *number) {
	int i = *((int*)number);
	sleep(rand()%30+1);
	
    printf("Student %d resumes programming\n", i);
    sem_wait(&availableChairs);

    printf("Student %d sits and waits\n", i);
    sleep(1);

    sem_wait(&TaDesk);
    sleep(1);

	sem_post(&availableChairs);

	int taSleepNumber;
	sem_getvalue(&TaSleep, &taSleepNumber);

    if(taSleepNumber == 0){
    	printf("Student %d awakes TA\n", i);
    	sem_post(&TaSleep);
    }

    printf("TA helps to student %d\n", i);
    sem_wait(&TaHelps);
    
    sem_post(&TaDesk);
	pthread_exit(NULL);
}




int main(int argc, char *argv[])
{
	int N = 10,i;
	int studentThreadNumbers[N];
	pthread_t studentThreads[N];
	pthread_t taThread;

	//There will be 3 chair for students to wait
    sem_init(&availableChairs, 0, 3);
    // only one chair for student which is get help from TA
    sem_init(&TaDesk, 0, 1);
    //TA sleep semaphore
    sem_init(&TaSleep, 0, 0);
    sem_init(&TaHelps, 0, 0);

	if(pthread_create(&taThread, NULL, &taFunction, 0) != 0) {
		perror("Error at creating ta thread.\n");
		exit(1);
	}

	for(i = 0; i < N; i++) {
		studentThreadNumbers[i] = i;
		if(pthread_create(&studentThreads[i], NULL, &studentFunction,studentThreadNumbers+i) != 0) {
			perror("Error at creating student thread.\n");
			exit(1);
		}

	}

	//Join student threads;
    for (i = 0; i < N; i ++) {
	     pthread_join(studentThreads[i], NULL);
    }

    sleep(5);

    //it is the indication that all students visited TA.
    continueExecution = 0;

    //wake up ta.
    sem_post(&TaSleep);

    //TA thread is joined
    pthread_join(taThread, NULL);

    sem_destroy(&availableChairs);
    sem_destroy(&TaDesk);
    sem_destroy(&TaSleep);
    sem_destroy(&TaHelps);
	return 0;
}
