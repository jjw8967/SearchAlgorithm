#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

#define SEARCH_NUM 1000000
#define BUFF_SIZE 1024
#define THREAD_NUM 1
#define PATH "../random/"

int STR_NUM;
int	NODE_NUM;
int finish = 0;
char **randomList,**searchList;
char *fname;
FILE *file;
int searchCount;

typedef struct Node{
	char* value;
	struct Node *left;
	struct Node *right;
}Node;



void* createNode(int num){

	Node* node = (Node*)malloc(sizeof(Node*));

	node->value=randomList[num];
	node->left = node->right = NULL;

	return node; 
}

void insertNode(Node **root, Node *node){
	
	if(*root == NULL){
		*root = node;
		return;
	}
	
	if(strcmp((*root)->value, node->value)>0)	//move left
		insertNode(&((*root)->left),node);
	else if(strcmp((*root)->value, node->value)<0)	//move right
		insertNode(&((*root)->right),node);

}

int searchNode(Node *root,char * str){
	if(root==NULL) return 0;
	if(strcmp(root->value,str)==0) return 1;
	searchCount++;	
	if(strcmp(root->value, str)>0)	//move left
		return searchNode(root->left,str);
	else	//move right
		return searchNode(root->right,str);

}

void *createTree(){
	int i;
	struct Node *temp,*root=NULL;
	
	for(i=0;i<NODE_NUM;i++){
        temp = createNode(i);
        insertNode(&root,temp);
    }
	return root;
}
void freeTree(Node* root){
	if(root == NULL) return;
	Node *temp = root;
	freeTree(root->left);
	freeTree(root->right);
	free(temp);

}
int main(int argc, char** argv){

	if(argc == 1){
        argv[1] = (char*)malloc(sizeof(char)*3);
        strcpy(argv[1],"256");
        argv[2] = (char*)malloc(sizeof(char)*7);
        strcpy(argv[2],"1000000");
    }else if(argc != 3){
		perror("Please arguments format is ( STR_NUM, NODE_NUM)\n");
		exit(0);
	}

	srand(time(NULL));

	Node* root;

	STR_NUM = atoi(argv[1]);
	NODE_NUM = atoi(argv[2]);
	randomList = (char**)malloc(sizeof(char*)*NODE_NUM);
	searchList = (char**)malloc(sizeof(char*)*SEARCH_NUM);


	int flen = strlen(argv[1])+strlen(argv[2])+strlen(PATH)+3;
	fname = (char*)malloc(sizeof(char)*flen);
	bzero(fname,flen);
	strcpy(fname,PATH);
	strcat(fname,argv[1]);
	strcat(fname,"_");
	strcat(fname,argv[2]);

	FILE *file = fopen(fname,"r");

	if(file == NULL){
		perror("Not found file");
		exit(1);
	}

	int i,j,len,correct=0;
	char* str = (char*)malloc(sizeof(char)*STR_NUM);
	struct timespec start, end, tstart,tend;
	double search_time, total_time,time=0;
	int value;

	Node* temp;


	// File Read Part
	clock_gettime(CLOCK_MONOTONIC, &start);

	for(i=0;i<NODE_NUM;i++){
		randomList[i] = (char*)malloc(sizeof(char)*STR_NUM);
		fscanf(file,"%s",randomList[i]);
	}

	fclose(file);
	
    strcat(fname,"_s");

	file = fopen(fname,"r");

	for(i=0;i<SEARCH_NUM;i++){
		searchList[i] = (char*)malloc(sizeof(char)*STR_NUM);
		fscanf(file,"%s",searchList[i]);
	}



	clock_gettime(CLOCK_MONOTONIC, &start);


	// Tree Construction Part
	root = createTree();


	clock_gettime(CLOCK_MONOTONIC, &end);
	
	time = end.tv_sec - start.tv_sec;
	time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("constructing time : %lf\n",time);
	
	
	printf("Success ready\n");



	search_time =0;

	clock_gettime(CLOCK_MONOTONIC,&tstart);

	for(i=0;i<SEARCH_NUM;i++){

		bzero(str,sizeof(str));

		str = searchList[i];
		
		searchCount=0;

		clock_gettime(CLOCK_MONOTONIC, &start);
		
		value = searchNode(root,str);

		clock_gettime(CLOCK_MONOTONIC, &end);
		
		search_time += end.tv_sec - start.tv_sec;
		search_time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
		
		if(value == 1) correct++;
	
	}

	clock_gettime(CLOCK_MONOTONIC, &tend);

	total_time = tend.tv_sec - tstart.tv_sec;
    total_time += (tend.tv_nsec - tstart.tv_nsec) / 1000000000.0;

	printf("Total Time is %lf\n",total_time);
	printf("Search Time is %lf\n",search_time);
	printf("One Time is %lf\n",search_time/SEARCH_NUM);
	printf("Correct Percent is %lf\n", (double)correct/(double)SEARCH_NUM*100);
	for(i=0;i<NODE_NUM;i++)
		free(randomList[i]);

	free(randomList);
	freeTree(root);

	free(searchList);

	free(str);
	fclose(file);
	return 0;
}	
