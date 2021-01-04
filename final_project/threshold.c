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
#define shan_ip "192.168.1.46"
#define shan_linux_ip "128.252.167.161"
#define shan_lab_ip "192.168.122.1"

char* ip = shan_lab_ip;

char* USERNAME = "shan";
char* PASSWORD = "shannon_super_secret_password";
#define PASSWORD_SIZE 29
#define NUM_ITERS 50
#define NUM_CHARS 27
#define BUFFER_PERCENT 0.9

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
  send(socket, pass, password_size, 0);
  clock_gettime(CLOCK_MONOTONIC, &req_before); //record time before
  result = read(socket, response, sizeof(response));
  clock_gettime(CLOCK_MONOTONIC, &req_after); //record time after

  if (result == -1) {
    perror("read failed");
    return -1;
  } else if (result == 0) {
    perror("server closed");
    return -1;
  }
  //printf("%s\n", response);

  double ret = req_after.tv_nsec - req_before.tv_nsec;
  if (ret < 0) {
    ret += 1000000000;
  }

  return ret / 1000;
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

  double test;
  double tests[PASSWORD_SIZE] = {0};
  char guess[BUF_SIZE];

  for (int i = 0; i < PASSWORD_SIZE; i++) {
      for (int j = 0; j < NUM_ITERS; j++) {
          strncpy(guess, PASSWORD, j);
          guess[j] = '\0';
          test = test_creds(USERNAME, guess, strlen(USERNAME), strlen(guess), sock);
          if (test == -1) exit(EXIT_FAILURE);
          tests[i] += test;
      }

      tests[i] /= PASSWORD_SIZE;
  }

  double threshold = 0;
  for (int i = 0; i < PASSWORD_SIZE - 1; i++) {
      threshold += tests[i+1] - tests[i];
  }
  threshold /= (PASSWORD_SIZE- 1);
  threshold *= BUFFER_PERCENT;

  close(sock);
  return threshold;
}