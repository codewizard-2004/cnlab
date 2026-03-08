#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h>
#include<sys/socket.h>


int main(){
	printf("CLIENT: STARTING\n");
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0){
		perror("CLIENT: FAILED TO CREATE SOCKET");
		exit(1);
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("CLIENT: FAILED TO CONNECT WITH SERVER");
		exit(1);
	}
	printf("CLIENT: CONNECTED TO SERVER\n");
	
	printf("SERVER: ");
	char buffer[1024];
	int bytes = recv(client_socket, buffer, sizeof(buffer)-1, 0);
	buffer[bytes] = '\0';
	printf("%s\n", buffer);
	
	while(1){
		printf("SMTP->");
		char command[1024];
		fgets(command, sizeof(buffer), stdin);
		send(client_socket, command, strlen(command), 0);
		
		if (strncmp(command, "QUIT", 4) == 0){
			break;
		}
		recv(client_socket,buffer,sizeof(buffer),0);
        	printf("%s",buffer);
		if(strncmp(command, "DATA", 4) == 0){
		
			char msg[1024];
			int bytes = recv(client_socket, msg, sizeof(msg)-1, 0);
			msg[bytes]  = '\0';
			printf("SERVER: %s\n", msg);
			
			while(1){
				char text[1024];
				fgets(text,sizeof(text),stdin);
				if (strcmp(text, ".\n") == 0){
					break;
				}
				send(client_socket, text, strlen(text), 0);
			}
		}
		recv(client_socket,buffer,sizeof(buffer),0);
            	printf("SERVER: %s\n",buffer);
	
	}
	close(client_socket);
	return  0;
}
