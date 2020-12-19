// Client side C/C++ program to demonstrate Socket programming 
// Reference: https://www.geeksforgeeks.org/socket-programming-cc/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <sys/un.h>
#include <netinet/ip.h>
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 39393

#define IP "128.252.167.161"
#define IP2 "127.0.0.1"

const int max_expected_args = 2;
   
int main(int argc, char const *argv[]) 
{
    if (argc > max_expected_args) {
	printf("Usage: ./client <quit (optional)>");
	exit(-1);
    }

    int sock = 0; 
    struct sockaddr_in serv_addr; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        perror("Error creating socket");
	exit(EXIT_FAILURE);
    } 
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    if(inet_pton(AF_INET, IP, &serv_addr.sin_addr) <= 0) {
	perror("Error in init_pton");
	exit(EXIT_FAILURE);
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        perror("Failed to connect");
	exit(EXIT_FAILURE);
    }

    //Send integer(s)
    int n = 17;
    int o = 65;
    int p = 24;

    send(sock, &n, sizeof(n), 0);
    send(sock, &o, sizeof(o), 0);
    send(sock, &p, sizeof(p), 0);

    printf("sent: %d\n", n);
    printf("sent: %d\n", o);
    printf("sent: %d\n", p);

    //If "quit" argument supplied, then send 418 to server to cause it to exit
    if (argc == 2) {
	if (strcmp(argv[1], "quit") == 0) {
	    int q = 418;
	    send(sock, &q, sizeof(q), 0);
	    printf("sent: %d (quit)\n", q);
	}
    }

    close(sock);
    return 0; 
} 