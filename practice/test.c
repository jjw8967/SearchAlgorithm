#include<stdio.h>
#include<stdlib.h>

int main(){
	char *str = (char*)malloc(sizeof(char)*1000);
	free(str);
	return 0;
}
