#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<signal.h>

#define BUFF_SIZE 1024
#define FILE_SERVER "/tmp/test_server"

int STR_NUM;
int	NODE_NUM;

void sighandle(int signum){
	printf("for server terminate, you must send 'end' string\n");
	
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

void insertNode(Node** root, Node* node){

	if(*root == NULL){
		*root = node;
		return;
	}
	
	if(strcmp((*root)->value, node->value)>0)	//move left
		insertNode(&((*root)->left),node);
	else if(strcmp((*root)->value, node->value)<0)	//move right
		insertNode(&((*root)->right),node);

	return;

}

int searchNode(Node* root, char* str){
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

int main(int argc, char** argv){

	if(argc != 3){
		perror("Please arguments format is ( STR_NUM, NODE_NUM)\n");
		exit(0);
	}

	srand(time(NULL));

	Node* root = NULL;

	STR_NUM = atoi(argv[1]);
	NODE_NUM = atoi(argv[2]);

	int i,j;
	char* str = (char*)malloc(sizeof(char)*STR_NUM);
	struct timespec start, end;
	int finish=0;
	int value;
	double time;

	Node* temp;
	FILE* out;

	signal(SIGINT,sighandle); // signal handler


	clock_gettime(CLOCK_MONOTONIC, &start);

	for(i=0;i<NODE_NUM;i++){
		temp = createNode();
		insertNode(&root,temp);
	}

	clock_gettime(CLOCK_MONOTONIC, &end);

	time = (end.tv_sec - start.tv_sec);
	time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("constructing time : %lf\n",time);
	
	printf("Success ready\n");

	//Accept socket
	/*
	while(!finish){
		client_addr_size = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr,&client_addr_size);
		
		if(-1 == client_socket){
			printf("Fail Connect Client\n");
			continue;
		}

		read(client_socket,buff_rcv,BUFF_SIZE);


		if(strcmp(buff_rcv,"end")==0)
			finish=1;
		


		start = clock()*1000000/(float)CLOCKS_PER_SEC;		
		
		value = searchNode(root,buff_rcv);		

		end = clock()*1000000/(float)CLOCKS_PER_SEC;

		printf("STR: %s, VALUE: %d, MICRO : %f\n",buff_rcv,value,end-start);	

		out = fopen("./binary_log.log","a");

		fprintf(out,"STR_NUM: %d, NODE_NUM: %d, MICRO: %f\n",STR_NUM,NODE_NUM,end-start);
		
		sprintf(buff_snd,"STR : %s, VALUE: %d, MICRO: %f",buff_rcv,value,end-start);
		write(client_socket, buff_snd,strlen(buff_snd)+1);
		close(client_socket);

		fclose(out);
	
	}
*/

	freeTree(root);

	free(str);

	return 0;
}	
