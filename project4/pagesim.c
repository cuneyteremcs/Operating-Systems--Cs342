#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

//STRUCTURES
struct node {
    char  *virtual;
    char *physical;
    int frameNo;
    long int index;
    struct node *next;
};// this is my Linked list to store words from input files
typedef struct node LIST;

int main(int argc, char *argv[])
{
	int M, A;

	M = atoi(argv[3]);
	A = atoi(argv[6]);
	char *R = argv[7];

	FILE *inFile1, *inFile2, *outFile;

	int pages[1024][1024];

	for(int i=0;i<1024;i++){
		for(int j=0;j<1024;j++){
			pages[i][j]=-1;
		}
	}

	if(R){

		char *r=argv[8];

		char x[8];
	        	strncpy(x, r, 7);
	        	printf("%s \n", r);
	        	printf("%s \n", x);
	        	long int j=strtol(x, NULL, 16);
	        	
	        	int pagex=j/1024, pagey=j%1024;
	        	printf("%d %d \n", pagex,pagey);
	        	for(int k=0;k<=pagex;k++){
	        		if(k==pagex){
	        			for(int l=0;l<pagey;l++){
	        				pages[k][l]=0;
	        			}
	        		}
	        		else{
	        			for(int l=0;l<1024;l++){
	        				pages[k][l]=0;
	        			}
	        		}
	        	}
	        	outFile=fopen(argv[4], "w");
		for(int i=0;i<10;i++){

			LIST *frame;
			frame=NULL;

			int frameCount=0;


			int random=rand() & 0xffffffff;
			char line3[11];
			sprintf(line3, "%X", random);
			//printf("%s\n", line2);
			char *line2=line3;
			for(int i=8; i>strlen(line2);i--){
				char cur[11];
				sprintf(cur,"0%s", line2);
				line2=cur;
			}
			printf("%s\n", line2);
			
	        	
	        	char x[8];
	        	strncpy(x, line2, 5);
	        	printf("%s \n", line2);
	        	long int i=strtol(x, NULL, 16);
	        	printf(" this is decimal %d \n", i);
	        	char offset[4];
	        	strncpy(offset, line2+5, 3);

	        	int valid=pages[i/1024][i%1024];

	        	if(A==1 && valid==0){
	        		if(frameCount==M){
	        			LIST *cur=frame;
	        			LIST *prev=cur;
	        				int exists=0;
	        				while(cur){
	        					if(cur->index==i){
	        						exists=1;
	        						fprintf(outFile, "%s %X%s\n", line2, cur->frameNo, offset);
	        					}
	        					prev=cur;
	        					cur=cur->next;

	        				}
	        				if(exists==0){
	        					prev->next=(LIST*)malloc(sizeof(LIST));
	        					prev->next->virtual=line2;
		        				prev->next->index=i;
		        				prev->next->frameNo=frame->frameNo;
		        				prev->next->next=NULL;
		        				LIST *temp=frame;
		        				frame=frame->next;
		        				free(temp);
		        				temp=NULL;
		        				fprintf(outFile, "%s %X%s X\n", line2, prev->next->frameNo, offset);
	        				}
	        		}
	        		else{
	        			if(frame==NULL){
	        				frame=(LIST*)malloc(sizeof(LIST));
	        				frame->virtual=line2;
	        				frame->index=i;
	        				frame->frameNo=frameCount+1;
	        				frame->next=NULL;
	        				fprintf(outFile, "%s %X%s X\n", frame->virtual, frame->frameNo, offset);
	        				frameCount++;
	        			}
	        			else{
	        				LIST *cur=frame;
	        				LIST *prev=cur;
	        				int exists=0;
	        				while(cur){
	        					if(cur->index==i){
	        						exists=1;
	        						fprintf(outFile, "%s %X%s\n", line2, cur->frameNo, offset);
	        					}
	        					prev=cur;
	        					cur=cur->next;

	        				}
	        				if(exists==0){
	        					prev->next=(LIST*)malloc(sizeof(LIST));
	        					prev->next->virtual=line2;
		        				prev->next->index=i;
		        				prev->next->frameNo=frameCount+1;
		        				prev->next->next=NULL;
		        				fprintf(outFile, "%s %X%s X\n", line2, prev->next->frameNo, offset);
		        				frameCount++;
	        				}

	        			}
	        		}
	        	}
	        	else if(A!=1 && valid==0){
	        		if(frameCount==M){
	        			LIST *cur;
	        				LIST *prev;
	        				LIST *change;
	        				LIST *temp;
	        				int count=0;
	        				if(frame->index==i){
	        					if(frameCount==1){
		        					frame->virtual=line2;
		        					fprintf(outFile, "%s %X%s\n", line2, frame->frameNo, offset);
	        					}
	        					else{
	        						LIST *change=frame;
	        						frame=frame->next;
	        						cur=frame->next;
	        						while(cur)
	        							cur=cur->next;
	        						change->virtual=line2;
	        						change->next=NULL;
	        						cur->next=change;

	        						fprintf(outFile, "%s %X%s\n", line2, change->frameNo,offset);

	        					}
	        				}
	        				else{
	        					cur=frame;
	        					prev=cur;
	        					cur=cur->next;
	        					int exists=0;
	        					int inserted=0;
	        					while(cur){
	        						if(cur->index==i){
	        							if(cur->next==NULL){
	        								cur->virtual=line2;
	        								inserted=1;
	        								fprintf(outFile, "%s %X%s\n", line2, cur->frameNo, offset);
	        							}
	        							else{
	        								exists=1;
	        								change= cur;
		        							cur=cur->next;
		        							prev->next=cur;
	        							}
	        							
	        						}
	        						prev=cur;
		        					cur=cur->next;
	        					}

	        					if(exists==0 && inserted==0){
		        					prev->next=(LIST*)malloc(sizeof(LIST));
		        					prev->next->virtual=line2;
			        				prev->next->index=i;
			        				prev->next->frameNo=frame->frameNo;
			        				prev->next->next=NULL;
			        				LIST *temp=frame;
			        				frame=frame->next;
			        				free(temp);
			        				temp=NULL;
			        				fprintf(outFile, "%s %X%s X\n", line2, prev->next->frameNo, offset);
		        				}
		        				if(exists==1){
		        					change->virtual=line2;
		        					change->next=NULL;
		        					prev->next=change;
		        					fprintf(outFile, "%s %X%s \n", line2, change->frameNo, offset);
		        				}

	        				}
	        		}
	        		else{
	        			if(frame==NULL){
	        				//printf("i am here %s\n", line2);
	        				frame=(LIST*)malloc(sizeof(LIST));
	        				frame->virtual=line2;
	        				frame->index=i;
	        				frame->frameNo=frameCount+1;
	        				frame->next=NULL;
	        				fprintf(outFile, "%s %X%s X\n", frame->virtual, frame->frameNo, offset);
	        				frameCount++;
	        			}
	        			else{
	        				LIST *cur;
	        				LIST *prev;
	        				LIST *change;
	        				LIST *temp;
	        				int count=0;
	        				if(frame->index==i){
	        					if(frameCount==1){
		        					frame->virtual=line2;
		        					fprintf(outFile, "%s %X%s \n", line2, frame->frameNo, offset);
	        					}
	        					else{
	        						LIST *change=frame;
	        						frame=frame->next;
	        						cur=frame->next;
	        						while(cur)
	        							cur=cur->next;
	        						change->virtual=line2;
	        						change->next=NULL;
	        						cur->next=change;

	        						fprintf(outFile, "%s %X%s \n", line2, change->frameNo,offset);

	        					}
	        				}
	        				else{
	        					cur=frame;
	        					prev=cur;
	        					cur=cur->next;
	        					int exists=0;
	        					int inserted=0;
	        					while(cur){
	        						if(cur->index==i){
	        							if(cur->next==NULL){
	        								cur->virtual=line2;
	        								inserted=1;
	        								fprintf(outFile, "%s %X%s \n", line2, cur->frameNo,offset);
	        							}
	        							else{
	        								exists=1;
	        								change= cur;
		        							cur=cur->next;
		        							prev->next=cur;
	        							}
	        							
	        						}
	        						prev=cur;
		        					cur=cur->next;
	        					}

	        					if(exists==0 && inserted==0){
		        					prev->next=(LIST*)malloc(sizeof(LIST));
		        					prev->next->virtual=line2;
			        				prev->next->index=i;
			        				prev->next->frameNo=frameCount+1;
			        				prev->next->next=NULL;
			        				fprintf(outFile, "%s %X%s X\n", line2, prev->next->frameNo, offset);
			        				frameCount++;
		        				}
		        				if(exists==1){
		        					change->virtual=line2;
		        					change->next=NULL;
		        					prev->next=change;
		        					fprintf(outFile, "%s %d \n", line2, change->frameNo, offset);
		        				}

	        				}

	        			}
	        		}
	        	}
	        	else{
	        		fprintf(outFile, "%s e \n", line2);
	        	}
			
		}
		fclose(outFile);

	}
	else{

		char line[23];
	
		inFile1=fopen(argv[1], "r");
	 	while(fgets(line, sizeof(line),inFile1)){
	        	
	        	char x[8], y[8];
	        	strncpy(x, line, 7);
	        	strncpy(y, line+11, 7);
	        	printf("%s", line);
	        	printf("%s-%s \n", x, y);
	        	long int i=strtol(x, NULL, 16);
	        	long int j=strtol(y, NULL, 16);
	        	
	        	int pagexx=i/1024, pagexy=i%1024, pageyx=j/1024, pageyy=j%1024-1;
	        	printf("%d %d %d %d \n", pagexx, pagexy, pageyx, pageyy);
	        	for(int k=pagexx;k<=pageyx;k++){
	        		if(pagexx==pageyx){
	        			for(int l=pagexy;l<pageyy;l++){
	        				pages[k][l]=0;
	        			}
	        		}
	        		else{
	        			if(k==pagexx){
	        				for(int l=pagexy;l<=1023;l++){
	        					pages[k][l]=0;
	        				}
	        			}
	        			else if(k==pageyx){
	        				for(int l=0;l<pageyy;l++){
	        					pages[k][l]=0;
	        				}

	        			}
	        			else{
	        				for(int l=0;l<=1023;l++){
	        					pages[k][l]=0;
	        				}

	        			}
	        		}
	        	}
		}
		fclose(inFile1);

		char line2[13];

		LIST *frame;
		frame=NULL;

		int frameCount=0;

		outFile=fopen(argv[4], "w");

		inFile2=fopen(argv[2], "r");
	 	while(fgets(line2, sizeof(line2),inFile2)){
	        	
	        	char x[8];
	        	strncpy(x, line2, 7);
	        	printf("%s", line2);
	        	printf("%s \n", x);
	        	long int i=strtol(x, NULL, 16);
	        	printf("%d \n", i);
	        	char offset[4];
	        	strncpy(offset, line2+7, 3);

	        	int valid=pages[i/1024][i%1024];

	        	if(A==1 && valid==0){
	        		if(frameCount==M){
	        			LIST *cur=frame;
	        			LIST *prev=cur;
	        				int exists=0;
	        				while(cur){
	        					if(cur->index==i){
	        						exists=1;
	        						fprintf(outFile, "%s %X%s\n", line2, cur->frameNo, offset);
	        					}
	        					prev=cur;
	        					cur=cur->next;

	        				}
	        				if(exists==0){
	        					prev->next=(LIST*)malloc(sizeof(LIST));
	        					prev->next->virtual=line2;
		        				prev->next->index=i;
		        				prev->next->frameNo=frame->frameNo;
		        				prev->next->next=NULL;
		        				LIST *temp=frame;
		        				frame=frame->next;
		        				free(temp);
		        				temp=NULL;
		        				fprintf(outFile, "%s %X%s X\n", line2, prev->next->frameNo, offset);
	        				}
	        		}
	        		else{
	        			if(frame==NULL){
	        				frame=(LIST*)malloc(sizeof(LIST));
	        				frame->virtual=line2;
	        				frame->index=i;
	        				frame->frameNo=frameCount+1;
	        				frame->next=NULL;
	        				fprintf(outFile, "%s %X%s X\n", frame->virtual, frame->frameNo, offset);
	        				frameCount++;
	        			}
	        			else{
	        				LIST *cur=frame;
	        				LIST *prev=cur;
	        				int exists=0;
	        				while(cur){
	        					if(cur->index==i){
	        						exists=1;
	        						fprintf(outFile, "%s %X%s\n", line2, cur->frameNo, offset);
	        					}
	        					prev=cur;
	        					cur=cur->next;

	        				}
	        				if(exists==0){
	        					prev->next=(LIST*)malloc(sizeof(LIST));
	        					prev->next->virtual=line2;
		        				prev->next->index=i;
		        				prev->next->frameNo=frameCount+1;
		        				prev->next->next=NULL;
		        				fprintf(outFile, "%s %X%s X\n", line2, prev->next->frameNo, offset);
		        				frameCount++;
	        				}

	        			}
	        		}
	        	}
	        	else if(A!=1 && valid==0){
	        		if(frameCount==M){
	        			LIST *cur;
	        				LIST *prev;
	        				LIST *change;
	        				LIST *temp;
	        				int count=0;
	        				if(frame->index==i){
	        					if(frameCount==1){
		        					frame->virtual=line2;
		        					fprintf(outFile, "%s %X%s\n", line2, frame->frameNo, offset);
	        					}
	        					else{
	        						LIST *change=frame;
	        						frame=frame->next;
	        						cur=frame->next;
	        						while(cur)
	        							cur=cur->next;
	        						change->virtual=line2;
	        						change->next=NULL;
	        						cur->next=change;

	        						fprintf(outFile, "%s %X%s\n", line2, change->frameNo,offset);

	        					}
	        				}
	        				else{
	        					cur=frame;
	        					prev=cur;
	        					cur=cur->next;
	        					int exists=0;
	        					int inserted=0;
	        					while(cur){
	        						if(cur->index==i){
	        							if(cur->next==NULL){
	        								cur->virtual=line2;
	        								inserted=1;
	        								fprintf(outFile, "%s %X%s\n", line2, cur->frameNo, offset);
	        							}
	        							else{
	        								exists=1;
	        								change= cur;
		        							cur=cur->next;
		        							prev->next=cur;
	        							}
	        							
	        						}
	        						prev=cur;
		        					cur=cur->next;
	        					}

	        					if(exists==0 && inserted==0){
		        					prev->next=(LIST*)malloc(sizeof(LIST));
		        					prev->next->virtual=line2;
			        				prev->next->index=i;
			        				prev->next->frameNo=frame->frameNo;
			        				prev->next->next=NULL;
			        				LIST *temp=frame;
			        				frame=frame->next;
			        				free(temp);
			        				temp=NULL;
			        				fprintf(outFile, "%s %X%s X\n", line2, prev->next->frameNo, offset);
		        				}
		        				if(exists==1){
		        					change->virtual=line2;
		        					change->next=NULL;
		        					prev->next=change;
		        					fprintf(outFile, "%s %X%s \n", line2, change->frameNo, offset);
		        				}

	        				}
	        		}
	        		else{
	        			if(frame==NULL){
	        				//printf("i am here %s\n", line2);
	        				frame=(LIST*)malloc(sizeof(LIST));
	        				frame->virtual=line2;
	        				frame->index=i;
	        				frame->frameNo=frameCount+1;
	        				frame->next=NULL;
	        				fprintf(outFile, "%s %X%s X\n", frame->virtual, frame->frameNo, offset);
	        				frameCount++;
	        			}
	        			else{
	        				LIST *cur;
	        				LIST *prev;
	        				LIST *change;
	        				LIST *temp;
	        				int count=0;
	        				if(frame->index==i){
	        					if(frameCount==1){
		        					frame->virtual=line2;
		        					fprintf(outFile, "%s %X%s \n", line2, frame->frameNo, offset);
	        					}
	        					else{
	        						LIST *change=frame;
	        						frame=frame->next;
	        						cur=frame->next;
	        						while(cur)
	        							cur=cur->next;
	        						change->virtual=line2;
	        						change->next=NULL;
	        						cur->next=change;

	        						fprintf(outFile, "%s %X%s \n", line2, change->frameNo,offset);

	        					}
	        				}
	        				else{
	        					cur=frame;
	        					prev=cur;
	        					cur=cur->next;
	        					int exists=0;
	        					int inserted=0;
	        					while(cur){
	        						if(cur->index==i){
	        							if(cur->next==NULL){
	        								cur->virtual=line2;
	        								inserted=1;
	        								fprintf(outFile, "%s %X%s \n", line2, cur->frameNo,offset);
	        							}
	        							else{
	        								exists=1;
	        								change= cur;
		        							cur=cur->next;
		        							prev->next=cur;
	        							}
	        							
	        						}
	        						prev=cur;
		        					cur=cur->next;
	        					}

	        					if(exists==0 && inserted==0){
		        					prev->next=(LIST*)malloc(sizeof(LIST));
		        					prev->next->virtual=line2;
			        				prev->next->index=i;
			        				prev->next->frameNo=frameCount+1;
			        				prev->next->next=NULL;
			        				fprintf(outFile, "%s %X%s X\n", line2, prev->next->frameNo, offset);
			        				frameCount++;
		        				}
		        				if(exists==1){
		        					change->virtual=line2;
		        					change->next=NULL;
		        					prev->next=change;
		        					fprintf(outFile, "%s %d \n", line2, change->frameNo, offset);
		        				}

	        				}

	        			}
	        		}
	        	}
	        	else{
	        		fprintf(outFile, "%s e \n", line2);
	        	}

		}
		fclose(inFile2);
		fclose(outFile);

	}

	
}
