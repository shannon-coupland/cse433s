// Server side C/C++ program to demonstrate Socket programming
// Reference: https://www.geeksforgeeks.org/socket-programming-cc/

#include <unistd.h> 
#include <stdio.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> //used to be in.h from ex code, ip.h?
#include <arpa/inet.h>
#include <sys/un.h>
#include <string.h>
#define PORT 39393

int main(int argc, char const *argv[]) 
{ 
    //int opt = 1;
    int server_fd, new_socket;
    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("Socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    //if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        //perror("setsockopt failed");
        //exit(EXIT_FAILURE);
    //} 
       
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }

    int ex = 0;
    while(!ex) {
    	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
     	                  (socklen_t*)&addrlen))<0) 
    	{ 
     	   perror("accept"); 
     	   exit(EXIT_FAILURE);
    	} else {
	   printf("New connection to socket\n");
	}

    	//Read integer(s)
    	//Note: I attempted to use fscanf() per the studio assignment, but couldn't make
    	//the client side work. I used recv() and send() instead.
    	int c = 0;
    	while(recv(new_socket, &c, sizeof(c), 0) > 0) {
		printf("received: %d\n", c);
		if (c == 418) {
		    printf("Server closing\n");
		    ex = 1;
		}
    	}
    }

    //unlink(MY_SOCK_PATH);
    return 0; 
} 