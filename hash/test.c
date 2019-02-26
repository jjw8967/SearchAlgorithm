#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define BUFFER_SIZE 10000000
#define SEARCH_NUM 10000000
#define PATH "../random/"

typedef struct Node{
	int len;
	char **list;
	char *value;
}Node;

int NODE_NUM;
int STR_LEN;
int conflict=0;
int moveSum=0;
int oneSlot = 0;

int searchNode(struct Node *root, char *str);
void insertNode(struct Node **root, char *str);
void freeTree(struct Node *root);
int hashFunc(char* str);

int main(int argc, char**argv){
	
	
	if(argc == 1){
		NODE_NUM =1000000;
		STR_LEN = 256;
	}else if(argc == 3){
		NODE_NUM = atoi(argv[2]);
		STR_LEN = atoi(argv[1]);
	}else{
        perror("please input argument (string length, node number)");
        exit(0);
	}
	
	char fname[30];

	sprintf(fname,"%s%d_%d",PATH,STR_LEN,NODE_NUM);

	FILE *file = fopen(fname,"r");
	
	if(file == NULL){
		printf("%s\n",fname);
		perror("File Error!\n");
		exit(1);
	}
	
	int i,j,key,len,value,correct=0;
	char *str=(char*)malloc(sizeof(char)*(STR_LEN));

	double searchTime=0,functionTime = 0,Time;
	struct timespec start, end;
	int cycle_start, cycle_end;
	
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

    strcat(fname,"_s");

    FILE *sFile = fopen(fname,"r");

	cycle_start = clock();
	clock_gettime(CLOCK_MONOTONIC, &start);
	

	for(i=0;i<SEARCH_NUM;i++){
	
		bzero(str,STR_LEN);
		fscanf(sFile,"%s",str);

		// Measure Search Time
		
		key = hashFunc(str);

		value = searchNode(table[key],str);
		
		if(value == 1) correct++;
	
	}

	
	clock_gettime(CLOCK_MONOTONIC, &end);

	cycle_end = clock();

	searchTime = end.tv_sec - start.tv_sec;
	searchTime += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

	fclose(sFile);
	
	printf("Buffer size is %d\n",BUFFER_SIZE);
	printf("Conflicts is %d ( %.2f %%) \n",conflict,(float)conflict/(float)NODE_NUM*100);
	int hits = NODE_NUM-conflict;
	printf("Hits is %d ( %.2f %%) \n",hits,(float)(hits)/(float)NODE_NUM*100);
	printf("Search Time is %lf\n",searchTime);
	printf("One Time is %lf\n",searchTime/SEARCH_NUM);
	printf("Correct Percent is %lf\n",(float)correct/(float)SEARCH_NUM*100);
	printf("Move Average is %f\n",(float)moveSum/SEARCH_NUM);
	printf("Number of Cycle is %d\n",cycle_end-cycle_start);

	for(i=0;i<BUFFER_SIZE;i++){
		if(table[i] ==NULL) continue;
		else if(table[i]->len==0)
			oneSlot++;
	}
	printf("Number of One Slot %d ( %.2f %%)\n",oneSlot,(float)oneSlot/hits*100);
	for(i=0;i<BUFFER_SIZE;i++) freeTree(table[i]);

	free(table);
	
	return 0;

}
// Liear Search
/*
int searchNode(struct Node *root, char *str){
	char **list = root->list;
	
	int i,len = root->len,value=0;

	for(i=0;i<len;i++){
		if(strcmp(list[i],str)==0){
			value =1;
			break;
		}
		moveSum++;
	}

	return value;
}
*/
//Binary Search Tree

int searchNode(struct Node *root, char *str){
	
	
	if(strcmp(root->value,str)==0)
		return 1;
		

	int first=0,value=0,last = root->len-1;
	char **list = root->list;	
    int mid = (first+last)/2;

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
		moveSum++;
    }

    return value;
	
}

void insertNode(Node **root, char *str){

	Node *node;
	if(*root == NULL){
		
		node = (Node*)malloc(sizeof(Node));
		node->value = (char*)malloc(sizeof(char)*STR_LEN);
		strcpy(node->value,str);
		node->list = (char**)malloc(sizeof(char*)*1);
		node->len = 0;

        (*root) = node;
		
		return;
    }
	

	node = (*root);
	node->len ++;
	node->list = (char**)realloc(node->list,sizeof(char*)*(node->len));
	node->list[node->len-1] = (char*)malloc(sizeof(char)*STR_LEN);
	strcpy(node->list[node->len-1],str);

	conflict++;

	return;
}

int hashFunc(char* str){
	unsigned int i,sum = 0,len = strlen(str)-1;
    int move = (len % 7),sel;

    for(i=0;i<len;i+=7){
        sel = i+move;

        sum += ((str[i]|str[sel])<< (i%30));

    }
/*
	for(i=0;i<len;i++){
		sum += ((str[i]|str[i+2]) << i%30);
	}
*/
    return sum%BUFFER_SIZE;

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
