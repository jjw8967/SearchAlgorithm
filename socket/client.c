#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/un.h>
#include<time.h>

#define BUFF_SIZE 1024
#define FILE_SERVER "/tmp/test_server"

int main(){

	int client_socket;

	struct sockaddr_un server_addr;

	char str[15];
	char buff[BUFF_SIZE+5];
	int start,end;
	while(1){

		start = clock()*1000000/CLOCKS_PER_SEC;

		client_socket = socket(PF_FILE, SOCK_STREAM, 0);

		if(-1 == client_socket)
		{
			printf("Fail create socket\n");
			exit(1);
		}

		memset(&server_addr,0,sizeof(server_addr));
		server_addr.sun_family = AF_UNIX;
		strcpy(server_addr.sun_path,FILE_SERVER);

		if(connect(client_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
		{
			printf("Fail connect\n");
			exit(1);
		}

		bzero(str,sizeof(str));
		scanf("%s",str);

		write(client_socket,str,strlen(str)+1);

		read(client_socket,buff,BUFF_SIZE);

		end = clock()*1000000/CLOCKS_PER_SEC;

		printf("%s\n",buff);
		printf("time : %d\n",end-start);
		close(client_socket);
	}
	return 0;
}
