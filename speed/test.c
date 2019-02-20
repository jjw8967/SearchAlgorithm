#include<stdio.h>
#include<time.h>

int sum(int a, int result){
	if(a == 1 ) return result;
	return sum(a-1, result+a);
}

int main(){
	int a = 100000;
	double start = clock()*1000/CLOCKS_PER_SEC;
	printf("SUM 1 ... %d  = %d\n",a,sum(a,0));
	double end = clock()*1000/CLOCKS_PER_SEC;
	printf("%lf\n",end-start);
	return 0;
}
