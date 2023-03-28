#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void error(const char *msg)
{
 	perror(msg);
	exit(1);
}


int main(int argc,char *argv[])
{
 	if(argc < 2)
	{
		fprintf(stderr , "port no not provided. program terminated\n");
		exit(1);
	}
	
	int sockfd , newsockfd , portno ,n;
	char buffer[225];
	
	struct sockaddr_in serv_addr , cli_addr;
	socklen_t clilen;

	sockfd = socket(AF_INET , SOCK_STREAM ,0);
	if(sockfd < 0)
	{
		error("error operating socket.");
	}
	bzero((char *) &serv_addr , sizeof(serv_addr));
	portno = atoi(argv[1]);
		
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr =INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd ,(struct sockaddr *) &serv_addr , sizeof(serv_addr)) <0)
 		error("Binding Failed.");


	listen(sockfd , 5);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd , (struct sockaddr *) &cli_addr ,&clilen);

	if(newsockfd <0)
	error("error on accept");

	int num1 ,num2,ans,choice;
	
      S: n = write(newsockfd,"enter first no : ",strlen("enter first no"));                       // ask for first no
	if(n<0) error("error writing to socket");
	read(newsockfd, &num1,sizeof(int));
	printf("client-first no is :%d\n",num1);                                                 //read first no

	n = write(newsockfd,"enter second no : ",strlen("enter second no"));                       // ask for second no
	if(n<0) error("error writing to socket");
	read(newsockfd, &num2,sizeof(int));
	printf("client-second no is :%d\n",num2);                                                 //read second no

	n = write(newsockfd,"enter your choice:\n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.exit\n",strlen("enter your choice: \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.exit\n"));
	if(n<0)error("error writing socket");
	read(newsockfd, &choice,sizeof(int));
	printf("client - choice is :%d\n",choice);

	switch(choice)
	{
		case 1: 
			ans = num1 + num2;
			break;

		case 2: 
			ans = num1 - num2;
			break;

		case 3: 
			ans = num1 * num2;
			break;

		case 4: 
			ans = num1/num2;
			break;

		case 5: 
			goto Q;
			break;
	}
	
	write(newsockfd ,&ans , sizeof(int));
	if(choice !=5)
	goto S;

Q:	close(newsockfd);
	close(sockfd);
	return 0;
}
