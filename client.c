/*
filename server_ipaddress portno

argv[0] file name
agrv[1] server_ipaddress
argv[2] portno 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //to write a fns like read ,write,close
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


void error(const char *msg)
{
 	perror(msg);
	exit(0);
}


int main(int argc,char *argv[])
{	
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	
	char buffer[225];

 	if(argc < 3)
	{
		fprintf(stderr , "usage %s hostname port\n",argv[0]);
		exit(0);
	}
	
	portno =atoi(argv[2]);
	sockfd = socket(AF_INET , SOCK_STREAM ,0);
	if(sockfd < 0)
	{
		error("error operating socket.");

		server = gethostbyname(argv[1]);
		if(server == NULL)
		{
			fprintf(stderr,"error,no such host");
		}
		bzero((char *) &serv_addr , sizeof(serv_addr));
		serv_addr.sin_family =AF_INET;
		bcopy((char *) server->h_addr , (char *) &serv_addr .sin_addr , server->h_length);
		serv_addr.sin_port = htons(portno);
		if(connect(sockfd ,(struct sockaddr *) &serv_addr ,sizeof(serv_addr))<0)
			error("connection failed");

		while(1)
		{
			bzero(buffer , 255);
			fgets(buffer ,255 ,stdin);
			n= write(sockfd ,buffer ,strlen(buffer));
			if(n<0)
				error("error on writing");
			bzero(buffer ,255);
			n=read(sockfd ,buffer ,255);
			if(n<0)
				error("error on reading");
			printf("server:%s" ,buffer);
			bzero(buffer ,255);
			fgets(buffer , 255,stdin);

			n=write(sockfd , buffer, strlen(buffer));
			if(n<0)
				error("error on writing");

			int i = strncmp("bye",buffer , 3);
			if(i==0)
			break;
		}

		close(sockfd);
		return 0;
	}
}
