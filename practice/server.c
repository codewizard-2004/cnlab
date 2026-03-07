#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h>
#include<sys/socket.h>

int main(){
	printf("SERVER: WAKING UP\n");
	// Create a server socket
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	// Create a server socket address
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	// Bind the server socket with server address
	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("SERVER: FAILED TO BIND SOCKET TO THE ADDRESS");
		exit(1);
	}
	// Listen for incomming client connections
	listen(server_socket, 5);
	printf("SERVER: LISTENING AT PORT 8080\n");
	
	// Accept incomming connections
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
	if (client_socket < 0){
		printf("SERVER: FAILED TO CONNECT WITH CLIENT");
		exit(1);
	}
	
	printf("SERVER: CONNECTED WITH CLIENT\n");
	
	//recieve message from the client
	char buffer[1024];
	recv(client_socket, buffer, sizeof(buffer), 0);
	printf("CIENT: %s\n", buffer);
	
	// send reply to client
	char reply[1024];
	printf("SERVER:");
	fgets(reply, 1024, stdin);
	send(client_socket, reply, strlen(reply), 0);
	
	close(client_socket);
	close(server_socket);

	return 0;
}
