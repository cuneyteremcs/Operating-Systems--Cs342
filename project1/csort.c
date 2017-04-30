#include <stdlib.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define QUEUE_NAME  "/child_queue"
#define QUEUE_PERMISSIONS 0660
#define MAX_SIZE    1024
#define MSG_STOP    "/master_queue"

struct Node{
    long int number;
    struct Node *next;
};

struct Node* sortList(struct Node* list){
    if(list == NULL || list->next == NULL)
        return list;

    struct Node *curr, *minimum;
    curr = list;
    minimum = list;
    while(curr != NULL) {
            if( curr->number < minimum->number)
                minimum = curr;
            curr = curr->next;
    }
    long int temp = list->number;
    list->number = minimum->number;
    minimum->number = temp; 
    list->next = sortList(list->next);
    return list;
}

int main(int argc , char *argv[]){
    if(argc != 4){
        printf("Error! argument number should be 4! \n");
        printf("csort <n> <bin_infile> <text_outfile> \n");
        exit(EXIT_FAILURE);
    }
    printf("Started\n");
    int i,n;

    //Set the parameters
    n = atoi(argv[1]);
    FILE *outputFile,*binaryFile = fopen(argv[2], "r");
    if( binaryFile == NULL )
    {
        printf("Error at opening input file \n");
        perror ( argv[2] ); 
        exit(EXIT_FAILURE);
    }

    int numberFromInfile = 0;
    char singleNumber[8];
    while(fgets(singleNumber, sizeof(singleNumber),binaryFile)){
            numberFromInfile++;
    }
    fclose(binaryFile);

    int k = numberFromInfile / n;
    int remaining = numberFromInfile - k*n + k;


    struct Node *singleHead = NULL;


    
    for (i = 0; i < n; i++){
        pid_t  pID;
        pID = fork();
        if (n < 0) { /* error occurred */
            fprintf(stderr, "Fork Failed");
            exit(-1);
        }
        else if (pID == 0) { /* child process */
            printf("child %d started\n",i );
            binaryFile = fopen(argv[2], "r");
            int q;
            for(q=0;q<k*i;q++)
                fgets(singleNumber, sizeof(singleNumber),binaryFile);
            
            if( i == n-1)
                k = remaining;

            for(q=0;q<k;q++){
                fgets(singleNumber, sizeof(singleNumber),binaryFile);
                long int number=atoi(singleNumber);
                struct Node* numberNode =(struct Node*)malloc(sizeof(struct Node));
                numberNode->number = number;
                if(singleHead == NULL){
                    singleHead = numberNode;
                    singleHead->next = NULL;
                }
                else{
                    numberNode->next = singleHead;
                    singleHead = numberNode;
                }
            }
            fclose(binaryFile);
            sortList(singleHead);

            while(singleHead != NULL){
                printf("Sorted binary number is %ld\n", singleHead->number);

/*
                int id;
                if(id = mq_open(mqName[i], O_RDWR | O_CREAT, QUEUE_PERMISSIONS, NULL) == -1){
                    perror("can not create msg queue\n");
                    exit(1);
                }
                mq_send(id, (char*)&singleHead->number, sizeof(singleHead->number), 0);
                mq_getattr(id, &mq_attr);
                mq_close(id);
*/
                singleHead = singleHead->next;
            }
            exit(0);
       
        }
        wait(NULL);
    }
    
    printf("End of creating fork\n");

    for(i = 0; i < n; i++){
        wait(NULL);
    }
    
    return 0;
}

