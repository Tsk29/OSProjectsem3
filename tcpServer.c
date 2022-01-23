#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;
	 //The SOCKADDR_IN structure specifies a transport address and port for the AF_INET address family

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET for the IPv4 protocols
	//SOCK_STREAM, for TCP application
	//we create a new socket
	// less than 0 means error.

	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	//socket is sucessfully created if socketid is >=0 (a int )
	printf("[+]The Server Socket is created.\n");
      
	memset(&serverAddr, '\0', sizeof(serverAddr));
	//It is used to fill a block of memory with a null value
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//provide ip address

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));//bind the socket IP address to specfic port
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1)
	//infinite loop created and executed until clients wants to exit 
	//inorder to handle multiple clients

	{
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		// used by a server to accept a connection request from a client
		if(newSocket < 0){//if returned id is <0 then we exit the loop
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);//inorder to close socket
			//child close listening socket 
			

			while(1){
				recv(newSocket, buffer, 1024, 0);//read incoming data from the newSocket
				// when :exit is given as input client is disconnected from server
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					
					printf("Client: %s\n", buffer);
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
}
