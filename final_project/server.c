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

#define BUF_SIZE 256
#define PORT 39393

#define SUCCESS "Successfully logged in!"
#define NO_USER "Username does not exist."
#define BAD_PASS "Wrong password - try again."

#define CREDS_SIZE 2
const char* CREDS[CREDS_SIZE][2] = {{"shan", "shannon_password_super_secret"},
                                    {"jason", "jason_password_super_secret"}};


int get_input(char* buf, int buf_size, int socket) {
  printf("entered check_creds");
  int result;

  if ((result = read(socket, buf, buf_size)) < 0) {
    perror("read failed");
    return -1;
  } else if (result == 0) {
    perror("client closed");
    return -1;
  }

  buf[buf_size - 1] = '\0';
  return 0;
}

int check_creds(char* username, char* password, int socket) {
  printf("entered check_creds");
  for (int i = 0; i < CREDS_SIZE; i++) {
    if (!strcmp(CREDS[i][0], username)) {
      if (!strcmp(CREDS[i][1], password)) {
        send(socket, SUCCESS, sizeof(SUCCESS), 0);
      } else {
        send(socket, BAD_PASS, sizeof(BAD_PASS), 0);
      }
      return 0;
    }
  }
  send(socket, NO_USER, sizeof(NO_USER), 0);
  return 0;
}

int main(int argc, char const* argv[]) {
  char* current_user;
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
    perror("accept failed");
  	exit(EXIT_FAILURE);
  } else {
    printf("New connection to socket\n");
  }

  while(1) {
    char username[BUF_SIZE];
    char password[BUF_SIZE];

    if (get_input(username, sizeof(username), new_socket) == -1
     || get_input(password, sizeof(password), new_socket) == -1) exit(EXIT_FAILURE);

    check_creds(username, password, new_socket);
  }

  return 0;
}
