#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define SEARCH_NUM 1000000
#define THREAD_NUM 1
#define PATH "../random/"

int STR_NUM;
int NODE_NUM;

void swap(char **x, char **y){
	char *temp = *x;
	*x = *y;
	*y = temp;
	
}

int search(char **list, char *str){
	int first = 0,last = NODE_NUM-1;
	int mid = (first+last)/2;
	int value = 0;

	while(last-first > 1){
		if(strcmp(list[mid],str)<0){
			first = mid;
			mid = (first+last)/2;
		}else if(strcmp(list[mid],str)>0){
			last = mid;
			mid = (first+last)/2;
		}else{
			value = 1;
			break;
		}
	}

	return value;
}
void quick(char **list, int left, int right){
	if(left>=right) return;
	int pivot = left;
	int low = left+1, high = right;

	while(low<=high){
		if(strcmp(list[low],list[pivot])>0){
			swap(&list[low],&list[high]);
			high--;
			continue;
		}
		low++;
	}
	
	swap(&list[pivot],&list[high]);
	pivot = high;

	quick(list,left,pivot-1);
	quick(list,pivot+1,right);
	
}

int main(int argc, char **argv){
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


    STR_NUM = atoi(argv[1]);
    NODE_NUM = atoi(argv[2]);
	char **sortedList = (char**)malloc(sizeof(char*)*NODE_NUM);

	char *fname;
	char *str = (char*)malloc(sizeof(char)*STR_NUM);
    int flen = strlen(argv[1])+strlen(argv[2])+strlen(PATH)+3;
	int i,value=0,correct=0;
    double Time;
    struct timespec start, end;

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

	for(i=0;i<NODE_NUM;i++){
        sortedList[i] = (char*)malloc(sizeof(char)*(STR_NUM+1));
        fscanf(file,"%s",sortedList[i]);
    }	

	fclose(file);

	strcat(fname,"_s");

    file = fopen(fname,"r"); 

	clock_gettime(CLOCK_MONOTONIC, &start);

	quick(sortedList,0,NODE_NUM-1);

	clock_gettime(CLOCK_MONOTONIC, &end);

    Time = end.tv_sec - start.tv_sec;
    Time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("constructing time : %lf\n",Time);
	
	clock_gettime(CLOCK_MONOTONIC, &start);

	Time=0;
	for(i=0;i<SEARCH_NUM;i++){
		bzero(str,STR_NUM);
		fscanf(file,"%s",str);
		//start
		clock_gettime(CLOCK_MONOTONIC, &start);
		
		value = search(sortedList,str);
		//end
		clock_gettime(CLOCK_MONOTONIC, &end);

		Time += end.tv_sec - start.tv_sec;
	    Time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

		if(value == 1)
			correct ++;
	}


    printf("Search Time is %lf\n",Time);
    printf("One Time is %lf\n",Time/SEARCH_NUM);
    printf("Correct Percent is %lf\n",(float)correct/(float)SEARCH_NUM*100);
	
	free(sortedList);

	fclose(file);

	free(fname);

	return 0;

}
