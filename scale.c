#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>
#include<pthread.h>


#define BUFF_SIZE 1024
#define THREAD_NUM 2

int STR_NUM;
int	NODE_NUM;
int finish = 0;

void sighandle(int signum){
	finish = 1;	
}

typedef struct Node{
	char* value;
	struct Node *left;
	struct Node *right;
}Node;



void* createNode(){
	char* str = (char*)malloc(sizeof(char)*STR_NUM);
	Node* node = (Node*)malloc(sizeof(Node*));

	int i;
	int random = rand() % STR_NUM +1;
	
	for(i=0;i<random;i++)
		str[i] = rand()%26+97;

	node->value=str;
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
	
	if(strcmp(root->value, str)>0)	//move left
		return searchNode(root->left,str);
	else	//move right
		return searchNode(root->right,str);

}

void freeTree(Node* root){
	if(root == NULL) return;
	Node *temp = root;
	freeTree(root->left);
	freeTree(root->right);
	free(temp);

}

void *createTree(void *data)
{

	int i,node_num = *(int *)data;

	printf("%d\n",node_num);
	struct Node *root = NULL, *temp;

	for(i=0;i<node_num;i++){
		temp = createNode();
		insertNode(&root,temp);
    }
	printf("thread finish\n");
	return (void*)root;
}

int main(int argc, char** argv){

	if(argc != 3){
		perror("Please arguments format is ( STR_NUM, NODE_NUM)\n");
		exit(0);
	}

	srand(time(NULL));

	Node* root[THREAD_NUM];

	STR_NUM = atoi(argv[1]);
	NODE_NUM = atoi(argv[2]);

	int i,j;
	char* str = (char*)malloc(sizeof(char)*STR_NUM);
	struct timespec start, end;
	double time;
	int value;

	Node* temp;
	FILE* out;

	signal(SIGINT,sighandle); // signal handler

	pthread_t thread[THREAD_NUM];

	int data = NODE_NUM/THREAD_NUM;
	for(i=0;i<THREAD_NUM;i++){
		if(pthread_create(&thread[i],NULL,createTree,(void*)&data)<0){
			fprintf(stderr,"thread%d create error :",i);
			exit(0);
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &start);

	for(i=0;i<THREAD_NUM;i++)
		pthread_join(thread[i],(void**)&root[i]);

	clock_gettime(CLOCK_MONOTONIC, &end);
	
	time = end.tv_sec - start.tv_sec;
	time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("constructing time : %lf\n",time);
	
	
	printf("Success ready\n");

	//Accept socket
	/*
	while(!finish){

		bzero(str,sizeof(str));	
		scanf("%s",str);
		
		start = clock()*1000000/(float)CLOCKS_PER_SEC;		
		
		value = searchNode(root1,str);

		end = clock()*1000000/(float)CLOCKS_PER_SEC;

		printf("STR: %s, VALUE: %d, MICRO : %f\n",str,value,end-start);	

		out = fopen("./binary_log.log","a");

		fprintf(out,"STR_NUM: %d, NODE_NUM: %d, MICRO: %f\n",STR_NUM,NODE_NUM,end-start);

		fclose(out);
	
	}
	*/
	for(i=0;i<THREAD_NUM;i++){
		freeTree(root[i]);
	}

	free(str);

	return 0;
}	
