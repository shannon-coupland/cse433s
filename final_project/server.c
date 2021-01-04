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

#define GOT_USER "Received username."
#define SUCCESS  "Successfully logged in!"
#define NO_USER  "Username does not exist."
#define BAD_PASS "Wrong password, stupid."

#define SLEEP_TIME 1000
#define CREDS_SIZE 2
char* CREDS[CREDS_SIZE][2] = {{"shan", "shannon_password_super_secret"},
                                    {"jason", "jason_password_super_secret"}};

int string_compare(char* str1, char* str2, int len1, int len2) {
    int i = 0;
    int min;
    if (len1 < len2) min = len1;
    else min = len2;

    while (i < min) {
        if (str1[i] != str2[i]) {
            return -1;
        }
        i++;
        //usleep(SLEEP_TIME);
    }

    if (len1 != len2) return -1;
    return 0;
}

int get_input(char* buf, int buf_size, int socket) {
    //printf("entered get_input, about to read\n");
    int result;

    if ((result = read(socket, buf, buf_size)) < 0) {
        perror("read failed");
        return -1;
    } else if (result == 0) {
        perror("client closed");
        return -1;
    }

    buf[buf_size - 1] = '\0';
    //printf("about to exit get_input\n");

    return 0;
}

int check_creds(int user_index, char* password, int socket) {
    //printf("entered check_creds\n");
    if (string_compare(CREDS[user_index][1], password, strlen(CREDS[user_index][1]), strlen(password)) == 0) {
        send(socket, SUCCESS, sizeof(SUCCESS), 0);
    } else {
        send(socket, BAD_PASS, sizeof(BAD_PASS), 0);
    }
    //printf("about to exit check_creds\n");
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

    char username[BUF_SIZE];
    char password[BUF_SIZE];
    int user_index;
    bool found;
    while(1) {
        if (get_input(username, sizeof(username), new_socket) == -1) {
            printf("get_input returned -1 - exiting program\n");
            exit (EXIT_FAILURE);
        }

        found = false;
        for (int i = 0; i < CREDS_SIZE; i++) {
            if (!string_compare(CREDS[i][0], username, strlen(CREDS[i][0]), strlen(username))) {
                user_index = i;
                found = true;
            }
        }
        if (!found) {
            send(socket, NO_USER, sizeof(NO_USER), 0);
        } else {
            send (new_socket, GOT_USER, sizeof(GOT_USER), 0);
        }

        if (get_input(password, sizeof(password), new_socket) == -1) {
            printf("get_input returned -1 - exiting program\n");
            exit(EXIT_FAILURE);
        }

        check_creds(user_index, password, new_socket);
    }

    return 0;
}
