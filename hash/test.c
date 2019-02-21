#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define BUFFER_SIZE 100000000
#define SEARCH_NUM 1000000
#define PATH "../random/"

int NODE_NUM;
int STR_LEN;
char **randomList;
int conflict=0;

typedef struct Node{
	struct Node *right;
	struct Node *left;
	char *value;
}Node;

char *createRand(int num);
int searchNode(struct Node *root, char *str);
void insertNode(struct Node **root, struct Node *node);
void *createNode(char *str);
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
	strcpy(fname,PATH);
	strcat(fname,argv[1]);
	strcat(fname,"_");
	strcat(fname,argv[2]);

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
		insertNode(&table[key],(Node *)createNode(str));
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

    file = fopen(fname,"r");


	for(i=0;i<SEARCH_NUM;i++){
		
		bzero(str,STR_LEN);
		fscanf(file,"%s",str);

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

	
	printf("Buffer size is %d\n",BUFFER_SIZE);
	printf("Conflicts is %d ( %.2f %%) \n",conflict,(float)conflict/(float)NODE_NUM*100);
	printf("Search Time is %lf\n",searchTime);
	printf("Function Time is %lf\n",functionTime);
	printf("One Time is %lf\n",searchTime/SEARCH_NUM);
	printf("Correct Percent is %lf\n",(float)correct/(float)SEARCH_NUM*100);

	for(i=0;i<BUFFER_SIZE;i++) freeTree(table[i]);

	free(fname);
	free(table);
	
	fclose(file);
	
	
	return 0;

}

int searchNode(struct Node *root, char *str){
	if(root==NULL) return 0;
	else if(strcmp(root->value,str)==0) return 1;
	else if(strcmp(root->value, str)>0)   //move left
		searchNode(root->left,str);
    else								  //move right
        searchNode(root->right,str);
	
}
void insertNode(struct Node **root, struct Node *node){
	if(*root == NULL){
        *root = node;
        return;
    }
	
	conflict++;
    if(strcmp((*root)->value, node->value)>0)   //move left
        insertNode(&((*root)->left),node);
    else if(strcmp((*root)->value, node->value)<0)  //move right
        insertNode(&((*root)->right),node);	
}

void *createNode(char *str){
	Node *temp = (Node*)malloc(sizeof(Node));
	temp->value = (char*)malloc(sizeof(char)*STR_LEN);
	strcpy(temp->value,str);
	temp->right=temp->left = NULL;

	return (void *)temp;
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
    freeTree(root->left);
    freeTree(root->right);
	free(temp->value);
	free(temp);

}

