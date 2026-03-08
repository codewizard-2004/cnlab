#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>


int main(){
	printf("SERVER: STARTING SERVER\n");
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0){
		perror("SERVER: FAILED TO CREATE SOCKET");
		exit(1);
	}
	
	struct sockaddr_in socket_addr;
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(8080);
	socket_addr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(server_socket, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) < 0){
		perror("SERVER: FAILED TO BIND SOCKET TO ADDRESS");
		exit(1);
	}
	
	listen(server_socket, 5);
	printf("SERVER: LISTENING AT PORT 8080\n");
	
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
	if (client_socket < 0){
		perror("SERVER: FAILED TO CONNECT WITH CLIENT");
		exit(1);
	}
	
	printf("SERVER: CONNECTED WITH CLIENT");
	
	char welcome[] = "SMTP SERVER READY\n";
	send(client_socket, welcome, strlen(welcome), 0);
	
	FILE *fp = fopen("mail.txt", "w");
	char ok[] = "OK\n";
	
	while(1){
		char buffer[1024];
		int bytes = recv(client_socket, buffer, sizeof(buffer)-1, 0);
		buffer[bytes] = '\0';
		
		printf("CLIENT: %s", buffer);
		
		if (strncmp(buffer, "HELO", 4) == 0){
			char msg[] = "HELLO FROM SERVER\n";
			send(client_socket, msg, strlen(msg), 0);
		}
		else if(strncmp(buffer, "MAIL FROM", 9) == 0){
			printf("%s\n", buffer);
			fprintf(fp, "%s\n", buffer);
			fflush(fp);
			send(client_socket, ok, strlen(ok),0);
		}
		else if(strncmp(buffer, "RCPT TO", 7) == 0){
			printf("%s\n", buffer);
			fprintf(fp, "%s\n", buffer);
			fflush(fp);
			send(client_socket, ok, strlen(ok),0);
		}
		else if(strncmp(buffer, "DATA", 4) == 0){
			char reply[] = "Send text line by line. Enter . to finish\n";
			send(client_socket, reply, strlen(reply), 0);
			while(1){
				char data[1024];
				int dataBytes = recv(client_socket, data, sizeof(data)-1,0);
				data[dataBytes] = '\0';
				printf("%s\n", data);
				
				if (strcmp(data, ".\n") == 0){
					char reply[] = "Message Accepted\n";
					send(client_socket, reply, strlen(reply), 0);
					break;
				}
				fprintf(fp, "%s\n", data);
				fflush(fp);
			
			}
		}
		else if(strncmp(buffer, "QUIT", 4) == 0){
			fclose(fp);
			char reply[] = "BYE BYE\n";
			send(client_socket, reply, strlen(reply), 0);
			break;
		}
		else{
			char err[] = "Invalid Command\n";
			send(client_socket, err, strlen(err), 0);
		}
		
	
	}
	
	close(client_socket);
	close(server_socket);


}
