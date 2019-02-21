#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define SEARCH_NUM 1000000

int main(int argc,char **argv){
	srand(time(NULL));

	int i,j,flen = strlen(argv[1])+strlen(argv[2])+3;
	int random;
	int NODE_NUM = atoi(argv[2]);
	int STR_NUM = atoi(argv[1]);
	char *fname = (char*)malloc(sizeof(char)*flen);
	char **buff = (char**)malloc(sizeof(char*)*NODE_NUM);
	char *str = (char*)malloc(sizeof(char)*STR_NUM);
	FILE *rFile,*sFile;

	strcpy(fname,argv[1]);
	strcat(fname,"_");
	strcat(fname,argv[2]);

	rFile = fopen(fname,"w");

	strcat(fname,"_s");

	sFile = fopen(fname,"w");

	for(i=0;i<NODE_NUM;i++){
		bzero(str,STR_NUM);
		random = rand()%STR_NUM+1;

		for(j=0;j<random;j++)
			str[j] = rand()%26+97;

		fprintf(rFile,"%s\n",str);
	
		buff[i] = (char*)malloc(sizeof(char)*STR_NUM);
		
		strcpy(buff[i],str);
	}
	
	for(i=0;i<SEARCH_NUM;i++){
		random = rand()%NODE_NUM;
		
		fprintf(sFile,"%s\n",buff[random]);
	}

	fclose(sFile);
	fclose(rFile);

	for(i=0;i<NODE_NUM;i++) free(buff[i]);
	free(buff[i]);
	free(fname);
	free(str);

	return 0;
}
