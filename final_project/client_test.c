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

//add your own ip here if you'd like to test!
#define jason_ip "192.168.122.1"
#define shan_ip "192.168.1.46"
#define shan_linux_ip "128.252.167.161"
#define shan_lab_ip "192.168.122.1"
char* ip = jason_ip; //change this to your ip

#define NUM_ITERS 50
#define NUM_CHARS 27
char CHARS[NUM_CHARS] = {'a','b','c','d','e','f','g','h','i',
                         'j','k','l','m','n','o','p','q','r',
                         's','t','u','v','w','x','y','z','_'};

struct timespec req_before, req_after;
const int max_expected_args = 2;


//tests validity of username (user) and password (pass) pair
//sends these to server and checks response
//returns -1 if read from server fails
//otherwise, returns time it takes for password to be verified by server
double test_creds(char* user, char* pass, int username_size, int password_size, int socket) {
    //send username
    int result;
    char response[BUF_SIZE];
    send(socket, user, username_size, 0);

    //wait for server before sending password
    if ((result = read(socket, response, sizeof(response))) == -1) {
        perror("read failed");
        return -1;
    } else if (result == 0) {
        perror("server closed");
        return -1;
    }

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

  double test, test1 = 0, test2 = 0, test3 = 0, test4 = 0, test5 = 0, test6 = 0,
               test7 = 0, test8 = 0, test9 = 0, test10 = 0, test11 = 0,
               test12 = 0, test13 = 0, test14 = 0, test15 = 0;

  char* username = "shan";
  char* password;

  for (int i = 0; i < NUM_ITERS; i++) {
    //printf("%d\n", i);

    //test1 (correct)
    password = "shannon_password_super_secret";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test1 += test;

    //test2 (incorrect, index 26 of 28)
    password = "shannon_password_super_secz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test2 += test;

    //test3 (incorrect, index 24 of 28)
    password = "shannon_password_super_sz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test3 += test;

    //test4 (incorrect, index 22 of 28)
    password = "shannon_password_superz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test4 += test;

    //test5 (incorrect, index 20 of 28)
    password = "shannon_password_supz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test5 += test;
    printf("test: %lf\n", test);

    //test6 (incorrect, index 18 of 28)
    password = "shannon_password_sz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test6 += test;

    //test7 (incorrect, index 16 of 28)
    password = "shannon_passwordz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test7 += test;

    //test8 (incorrect, index 14 of 28)
    password = "shannon_passwoz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test8 += test;

    //test9 (incorrect, index 12 of 28)
    password = "shannon_passz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test9 += test;

    //test10 (incorrect, index 10 of 28)
    password = "shannon_paz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test10 += test;

    //test11 (incorrect, index 8 of 28)
    password = "shannon_z";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test11 += test;

    //test12 (incorrect, index 6 of 28)
    password = "shannoz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test12 += test;

    //test13 (incorrect, index 4 of 28)
    password = "shanz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test13 += test;

    //test14 (incorrect, index 2 of 28)
    password = "shz";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test14 += test;

    //test15 (incorrect, index 0 of 28)
    password = "z";
    test = test_creds(username, password, strlen(username), strlen(password), sock);
    if (test == -1) exit(EXIT_FAILURE);
    test15 += test;
  }

  test1 = test1 / NUM_ITERS;
  test2 = test2 / NUM_ITERS;
  test3 = test3 / NUM_ITERS;
  test4 = test4 / NUM_ITERS;
  test5 = test5 / NUM_ITERS;
  test6 = test6 / NUM_ITERS;
  test7 = test7 / NUM_ITERS;
  test8 = test8 / NUM_ITERS;
  test9 = test9 / NUM_ITERS;
  test10 = test10 / NUM_ITERS;
  test11 = test11 / NUM_ITERS;
  test12 = test12 / NUM_ITERS;
  test13 = test13 / NUM_ITERS;
  test14 = test14 / NUM_ITERS;
  test15 = test15 / NUM_ITERS;
  printf("test1 (correct) average time (us): %lf\n", test1);
  printf("test2 (incorrect, index 26 of 28) average time (us): %lf\n", test2);
  printf("test3 (incorrect, index 24 of 28) average time (us): %lf\n", test3);
  printf("test4 (incorrect, index 22 of 28) average time (us): %lf\n", test4);
  printf("test5 (incorrect, index 20 of 28) average time (us): %lf\n", test5);
  printf("test6 (incorrect, index 18 of 28) average time (us): %lf\n", test6);
  printf("test7 (incorrect, index 16 of 28) average time (us): %lf\n", test7);
  printf("test8 (incorrect, index 14 of 28) average time (us): %lf\n", test8);
  printf("test9 (incorrect, index 12 of 28) average time (us): %lf\n", test9);
  printf("test10 (incorrect, index 10 of 28) average time (us): %lf\n", test10);
  printf("test11 (incorrect, index 8 of 28) average time (us): %lf\n", test11);
  printf("test12 (incorrect, index 6 of 28) average time (us): %lf\n", test12);
  printf("test13 (incorrect, index 4 of 28) average time (us): %lf\n", test13);
  printf("test14 (incorrect, index 2 of 28) average time (us): %lf\n", test14);
  printf("test15 (incorrect, index 0 of 28) average time (us): %lf\n", test15);

  close(sock);
  return 0;
}
