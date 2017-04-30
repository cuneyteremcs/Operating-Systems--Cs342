#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct node
{
    char line[256];
    struct node * next;
} ;

struct node* readFile(char* fileName){
    struct node* head = NULL;
    FILE *file;
    char singleLine[256];
    file = fopen(fileName,"r");
	if( file == NULL )
	{
		perror ( fileName );
		exit(EXIT_FAILURE);
	}
	struct node* current = head;
	while (fgets (singleLine, 256, file) != NULL )
	{
	    if(current == NULL){
            head = (struct node * ) malloc(sizeof(struct node));
            current = head;
	    }
	    else{
            current->next = (struct node * ) malloc(sizeof(struct node));
            current = current->next;
            current->next = NULL;
	    }
		strcpy(current->line, singleLine);
	}
	fclose(file);
	printf("Reading %s finished\n",fileName);
	return head;
}
void writeFile(struct node* list,char* fileName){
    FILE *file = fopen(fileName,"w");
	if( file == NULL )
	{
		perror ( fileName );
		exit(EXIT_FAILURE);
	}
    while(list != NULL){
        fputs(list->line,file);
        list = list->next;
    }
    fclose(file);
    printf("Writing %s finished\n",fileName);
}
int isUniqe(char *line ,struct node* list){
    while(list != NULL){
        if(strcmp(line,list->line) == 0)
            return 0;
        list = list->next;
    }
    return 1;
}

struct node* sortList(struct node* list){
    if(list == NULL || list->next == NULL)
        return list;

    struct node *curr, *largest;
    curr = list;
    largest = list;
    while(curr != NULL) {
            if( strcmp(curr->line,largest->line) < 0)
                largest = curr;
            curr = curr->next;
    }
    char singleLine[256];
    strcpy(singleLine,list->line);
    strcpy(list->line,largest->line);
    strcpy(largest->line ,singleLine);
    list->next = sortList(list->next);
    return list;
}

int main(int argc , char *argv[])
{

	if(argc != 4){
		printf("%d",argc);
		perror ( "There should be 3 argument. 2 input file and 1 output file name" );
		exit(EXIT_FAILURE);	
	}
	//Attributes
	struct node *listA,*listB,*differenceList = NULL;
	char singleLine[256];

	//Read files and create linked list
	listA= readFile(argv[1]);
	listB=readFile(argv[2]);

	//find the difference
    struct node *outputList = differenceList;
    while(listA != NULL){
        if(isUniqe(listA->line,listB)){
            if(outputList == NULL){
                differenceList = (struct node * ) malloc(sizeof(struct node));
                outputList = differenceList;
            }
            else{
                outputList->next = (struct node * ) malloc(sizeof(struct node));
                outputList = outputList->next;
                outputList->next = NULL;
            }
            strcpy(outputList->line, listA->line);
        }
        listA = listA->next;
    }

    //sort the difference
    differenceList = sortList(differenceList);
    //write the difference
    writeFile(differenceList,argv[3]);
	return 0;
}
