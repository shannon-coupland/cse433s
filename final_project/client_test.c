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
#include <time.h>

#define BUF_SIZE 256
#define PORT 39393
#define jason_ip "192.168.0.190"
#define shan_ip "192.168.0.161"
#define shan_linux_ip "128.252.167.161"

char* ip = shan_linux_ip;

#define NUM_ITERS 10000
struct timespec req_before, req_after;
const int max_expected_args = 2;

double test_creds(char* user, char* pass, int username_size, int password_size, int socket) {
  //printf("entered test_creds\n");
  int result;
  char response[BUF_SIZE];

  //send username
  send(socket, user, username_size, 0);

  //wait for server before sending password
  if ((result = read(socket, response, sizeof(response))) == -1) {
    perror("read failed");
    return -1;
  } else if (result == 0) {
    perror("server closed");
    return -1;
  }
  //printf("%s\n", response);

  //send password
  clock_gettime(CLOCK_MONOTONIC, &req_before); //record time before
  send(socket, pass, password_size, 0);
  if ((result = read(socket, response, sizeof(response))) == -1) {
    perror("read failed");
    return -1;
  } else if (result == 0) {
    perror("server closed");
    return -1;
  }
  clock_gettime(CLOCK_MONOTONIC, &req_after); //record time after
  //printf("%s\n", response);

  return (req_after.tv_nsec - req_before.tv_nsec);
}

int main(int argc, char const *argv[]) {
  if (argc > max_expected_args || argc < 1) {
    printf("Usage: ./client <quit (optional)>");
    exit(EXIT_FAILURE);
  }

  int sock = 0;
  struct sockaddr_in serv_addr;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Error creating socket");
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
	  perror("Error in init_pton");
	  exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Failed to connect");
	  exit(EXIT_FAILURE);
  }

  printf("connected\n");

  double test, test1 = 0, test2 = 0, test3 = 0;
  char* username = "shan";
  char* password = "shannon_password_super_secret";

  for (int i = 0; i < NUM_ITERS; i++) {
    //test1 (correct password)
    test = test_creds(username, password, sizeof(username), sizeof(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test1 += test;

    //test2 (incorrect password, index 0 of 28)
    password = "wrong_password";
    test = test_creds(username, password, sizeof(username), sizeof(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test2 += test;

    //test3 (incorrect password, index 22 of 28)
    password = "shannon_password_super_wrong_password";
    test = test_creds(username, password, sizeof(username), sizeof(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test3 += test;
  }

  test1 = test1 / NUM_ITERS;
  test2 = test2 / NUM_ITERS;
  test3 = test3 / NUM_ITERS;
  printf("test1 (correct password) average time (ns): %lf\n", test1);
  printf("test2 (incorrect password, index 0) average time (ns): %lf\n", test2);
  printf("test3 (incorrect password, index 22 of 28) average time (ns): %lf\n", test3);

  close(sock);
  return 0;
}
