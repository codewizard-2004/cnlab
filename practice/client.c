#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h>
#include<sys/socket.h>


int main(){
	printf("CLIENT: STARTING THE CLIENT\n");
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("CLIENT: FAILED TO CONNECT TO SERVER");
		exit(1);
	}
	
	printf("CLIENT: CONNECTED TO SERVER\n");
	
	// send a message to server
	char msg[] = "Good Morning server, I am your client.";
	printf("CLIENT: SENDING MESSAGE TO THE SERVER:  %s\n", msg);
	send(client_socket, msg, strlen(msg), 0);
	
	// recieve msg from server
	char buffer[1024];
	recv(client_socket, buffer, sizeof(buffer), 0);
	printf("SERVER: %s", buffer);
	
	close(client_socket);
	

	return 0;
	}
