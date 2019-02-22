#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define PATH "../random/"
#define TEST_SIZE 3

int STR_LEN,NODE_NUM;

int hashFunction(char *str,int size){
	unsigned int i,sum = 0,len = strlen(str);
	int move = len % 7,sel;
	
	for(i=0;i<len-1;i+=7){
		sel = i+move;
		
		sum += ((str[i]*str[sel]*str[(i+sel)/2]) << (i%30));
	}
	return sum%size;
}
int main(int argc,char **argv){

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

	int buffer_size[TEST_SIZE] = {1000000,10000000,100000000};
	char *str = (char*)malloc(sizeof(char)*STR_LEN);
	int i,size,key,conflict,hit,j;
    char fname[30];
	char **inputList = (char**)malloc(sizeof(char*)*NODE_NUM);
	int *buffer;
	struct timespec start,end;
	float total;

    sprintf(fname,"%s%d_%d",PATH,STR_LEN,NODE_NUM);
	

    FILE *file = fopen(fname,"r");

    if(file == NULL){
        printf("%s\n",fname);
        perror("File Error!\n");
        exit(1);
    }
	
	for(i=0;i<NODE_NUM;i++){
		inputList[i] = (char*)malloc(sizeof(char)*STR_LEN);

		bzero(str,STR_LEN);
		fscanf(file,"%s",str);
		
		strcpy(inputList[i],str);
	}

	fclose(file);

	for(i=0;i<TEST_SIZE;i++){
		size = buffer_size[i];
		buffer = (int*)malloc(sizeof(int)*size);
		memset(buffer,0,sizeof(int)*size);		

		hit = conflict = 0;
		total=0;

		for(j =0 ; j<NODE_NUM;j++){
			
			clock_gettime(CLOCK_MONOTONIC, &start);
			
			key = hashFunction(inputList[j],size);

			clock_gettime(CLOCK_MONOTONIC, &end);
			
			total += end.tv_sec - start.tv_sec;
	        total += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
			
			if(buffer[key] == 0){
				hit ++;
				buffer[key] = 1;
			}else if(buffer[key] == 1)conflict++;
		}

		free(buffer);
		printf("Buffer Size : %d , Hit Rate : %f, Conflict Rate : %f, ",size,(float)hit/NODE_NUM*100,(float)conflict/NODE_NUM*100);
		printf("Total Compute Time : %f , One Time: %f\n",total,total/NODE_NUM);
	}
	
	for(i=0;i<NODE_NUM;i++) free(inputList[i]);
	free(inputList);

	return 0;
}
