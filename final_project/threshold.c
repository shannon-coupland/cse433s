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

#define NUM_ITERS 10
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
    //printf("sent username\n");

    //wait for server before sending password
    if ((result = read(socket, response, sizeof(response))) == -1) {
        perror("read failed");
        return -1;
    } else if (result == 0) {
        perror("server closed");
        return -1;
    }
    //printf("received response: %s\n", response);

    //send password
    send(socket, pass, password_size, 0);
    //printf("sent password\n");
    clock_gettime(CLOCK_MONOTONIC, &req_before); //record time before
    result = read(socket, response, sizeof(response));
    clock_gettime(CLOCK_MONOTONIC, &req_after); //record time after
    //printf("received response: %s\n", response);

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

    //printf("ret is %lf\n", ret/1000);
    return ret / 1000;
}

int main(int argc, char const *argv[]) {
    if (argc > max_expected_args || argc < 1) {
        printf("Usage: ./threshold");
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

    int NUM_TESTS = strlen(PASSWORD);
    double test;
    double tests[NUM_TESTS];
    char guess[BUF_SIZE];

    for (int i = 0; i < NUM_TESTS; i++) {
        //printf("i = %d\n", i);
        strncpy(guess, PASSWORD, i+1);
        guess[i+1] = '|';
        guess[i+2] = '\0';
        printf("\n");
        printf("guessing password %s\n", guess);
        tests[i] = 0;

        for (int j = 0; j < NUM_ITERS; j++) {
            //printf("j = %d\n", j);
            test = test_creds(USERNAME, guess, strlen(USERNAME), strlen(guess), sock);
            printf("test = %lf\n", test);
            
            if (test == -1) exit(EXIT_FAILURE);
            tests[i] += test;
        }

        tests[i] /= NUM_ITERS;
    }

    printf("About to calculate threshold\n");
    double threshold = 0;
    double diff;
    for (int i = 0; i < NUM_TESTS - 1; i++) {
        diff = tests[i+1] - tests[i];
        if (threshold == 0 || diff < threshold) {
          threshold = diff;
        }
    }
    threshold /= (NUM_TESTS - 1);
    threshold *= BUFFER_PERCENT;

    close(sock);

    printf("Threshold is %lf\n", threshold);
    return threshold;
}
