#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define BUFFER_SIZE 100000000
#define SEARCH_NUM 1000000
#define PATH "../random/"

typedef struct Node{
	int len;
	char **list;
}Node;

int NODE_NUM;
int STR_LEN;
int conflict=0;
int moveSum=0;

int searchNode(struct Node *root, char *str);
void insertNode(struct Node **root, char *str);
void freeTree(struct Node *root);
int hashFunc(char* str);

int main(int argc, char**argv){
	
	
	if(argc == 1){
		argv[1] = (char*)malloc(sizeof(char)*3);
		strcpy(argv[1],"256");
		argv[2] = (char*)malloc(sizeof(char)*7);
		strcpy(argv[2],"1000000");
	}
	else if(argc != 3){
		perror("please input argument (string length, node number)");
		exit(0);
	}
	
	char *fname = (char*)malloc(sizeof(char)*(strlen(PATH)+strlen(argv[1])+strlen(argv[2])+3));
	
	sprintf(fname,"%s%s_%s",PATH,argv[1],argv[2]);

	FILE *file = fopen(fname,"r");
	
	if(file == NULL){
		printf("%s\n",fname);
		perror("File Error!\n");
		exit(1);
	}
	
	NODE_NUM = atoi(argv[2]);
	STR_LEN = atoi(argv[1]);
	
	int i,j,key,len,value,correct=0;
	char *str=(char*)malloc(sizeof(char)*(STR_LEN+1));
	double searchTime=0,functionTime = 0,Time;
	struct timespec start, end, fstart, fend;
	
	clock_gettime(CLOCK_MONOTONIC, &start);

	Node **table = (Node**)malloc(sizeof(Node*)*BUFFER_SIZE);
	
	for(i=0;i<NODE_NUM;i++){
		bzero(str,STR_LEN);
		fscanf(file,"%s",str);

		key = hashFunc(str);
		insertNode(&table[key],str);
	}

	fclose(file);
	
	clock_gettime(CLOCK_MONOTONIC, &end);

	Time = end.tv_sec - start.tv_sec;
    Time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("constructing time : %lf\n",Time);

	printf("Search Node is %d\n",SEARCH_NUM);
	printf("Ready!\n");

	srand(time(NULL));
	
	searchTime = 0;

    strcat(fname,"_s");

    FILE *sFile = fopen(fname,"r");


	for(i=0;i<SEARCH_NUM;i++){
		
		bzero(str,STR_LEN);
		fscanf(sFile,"%s",str);

		//Search
		clock_gettime(CLOCK_MONOTONIC, &fstart);
		
		key = hashFunc(str);

		clock_gettime(CLOCK_MONOTONIC, &fend);

		clock_gettime(CLOCK_MONOTONIC, &start);

		value = searchNode(table[key],str);
	
		clock_gettime(CLOCK_MONOTONIC, &end);
		

		functionTime += fend.tv_sec - fstart.tv_sec;
        functionTime += (fend.tv_nsec - fstart.tv_nsec) / 1000000000.0;

		
		searchTime += end.tv_sec - start.tv_sec;
	    searchTime += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
		
		if(value == 1) correct++;
	
	}

	fclose(sFile);
	
	printf("Buffer size is %d\n",BUFFER_SIZE);
	printf("Conflicts is %d ( %.2f %%) \n",conflict,(float)conflict/(float)NODE_NUM*100);
	printf("Search Time is %lf\n",searchTime);
	printf("Function Time is %lf\n",functionTime);
	printf("One Time is %lf\n",searchTime/SEARCH_NUM);
	printf("Correct Percent is %lf\n",(float)correct/(float)SEARCH_NUM*100);
	printf("Move Average is %d\n",moveSum/SEARCH_NUM);

	for(i=0;i<BUFFER_SIZE;i++) freeTree(table[i]);

	free(fname);
	free(table);
	
	return 0;

}

int searchNode(struct Node *root, char *str){
	char **list = root->list;
	int first = 0,last = root->len-1;
    int mid = (first+last)/2;
    int value = 0;

    while(first <= last){
        if(strcmp(list[mid],str)==0){
            value = 1;
            break;
        }
        else if(strcmp(list[mid],str)<0){
            first = mid + 1;
            mid = (first+last)/2;
        }else if(strcmp(list[mid],str)>0){
            last = mid - 1;
            mid = (first+last)/2;
        }
    }

    return value;
	
}
void insertNode(Node **root, char *str){

	if(*root == NULL){
		(*root) = (Node*)malloc(sizeof(Node));
		(*root)->list = (char**)malloc(sizeof(char*)*1);
		(*root)->list[0] = (char*)malloc(sizeof(char)*STR_LEN);
		strcpy((*root)->list[0],str);
		(*root)->len = 1;
        
		return;
    }

	Node *node = (*root);
	node->len +=1;
	node->list = (char**)realloc(node->list,sizeof(char*)*(node->len));
	node->list[node->len-1] = (char*)malloc(sizeof(char)*STR_LEN);
	strcpy(node->list[node->len-1],str);

	conflict++;

	return;
}

int hashFunc(char* str){
	int i,sum=0;
	int len = strlen(str);
	for(i=0;i<len;i++){
		sum += (str[i]*str[i]+str[i]*i);
	}
	return (sum*strlen(str))%BUFFER_SIZE;
}

void freeTree(Node* root){
    if(root == NULL) return;
	Node *temp = root;
    int i;

	for(i=0;i<temp->len;i++){
		free(temp->list[i]);
	}
	free(temp->list);
	free(temp);

}
