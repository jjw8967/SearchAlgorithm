#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define SEARCH_NUM 1000000
#define THREAD_NUM 1
#define PATH "../random/"

int STR_NUM;
int NODE_NUM;
int moveSum=0;

void swap(char **x, char **y){
	char *temp = *x;
	*x = *y;
	*y = temp;
	
}

int search(char **list, char *str){
	int first = 0,last = NODE_NUM-1;
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
		moveSum++;
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
		STR_NUM = 256; 
        NODE_NUM = 1000000;
    }else if(argc == 3){
		STR_NUM = atoi(argv[1]);
		NODE_NUM = atoi(argv[2]);
    }else{
		perror("Please arguments format is ( STR_NUM, NODE_NUM)\n");
		exit(0);
	}

    srand(time(NULL));

	char **sortedList = (char**)malloc(sizeof(char*)*NODE_NUM);

	char fname[30];
	char *str = (char*)malloc(sizeof(char)*STR_NUM);
	int i,value=0,correct=0;
    double Time;
    struct timespec start, end;
	int cycle_start,cycle_end;
    
	sprintf(fname,"%s%d_%d",PATH,STR_NUM,NODE_NUM);

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

    FILE *sFile = fopen(fname,"r");

	clock_gettime(CLOCK_MONOTONIC, &start);

	//quick(sortedList,0,NODE_NUM-1);		I used sorted data in file

	clock_gettime(CLOCK_MONOTONIC, &end);

    Time = end.tv_sec - start.tv_sec;
    Time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("constructing time : %lf\n",Time);

	cycle_start = clock();
	clock_gettime(CLOCK_MONOTONIC, &start);

	Time=0;
	
	for(i=0;i<SEARCH_NUM;i++){
		bzero(str,STR_NUM);
		fscanf(sFile,"%s",str);

		// Search String
		
		value = search(sortedList,str);
		
		if(value == 1)
			correct ++;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	cycle_end = clock();

	Time += end.tv_sec - start.tv_sec;
	Time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;


	fclose(sFile);

    printf("Search Time is %lf\n",Time);
    printf("One Time is %lf\n",Time/SEARCH_NUM);
    printf("Correct Percent is %lf\n",(float)correct/(float)SEARCH_NUM*100);
    printf("Move Average is %f\n",(float)moveSum/SEARCH_NUM);
	printf("Number of Cycle is %d\n",cycle_end-cycle_start);

    for(i=0;i<NODE_NUM;i++){
        free(sortedList[i]);
    } 

	free(sortedList);

	return 0;

}
