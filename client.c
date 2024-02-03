/*
 * client.c -- TCP Socket Client
 * 
 * adapted from: 
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
/**
 * Retrieves a file
 * @param source file on server
 * @param destination file in local
 * @param socket_desc socket to use
 */
void handle_get(char source[], char destination[], int socket_desc) {
    char command[103] = "g ";
    strcat(command, source);
    if (send(socket_desc, command, strlen(command), 0) < 0) {
        printf("Unable to send message\n");
    }

    char server_message[9001];
    memset(server_message, '\0', sizeof(server_message));

    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
        printf("Error while receiving server's msg\n");
    }
    printf("Received file. Stored at %s\n", destination);
    FILE* file = fopen(destination, "w");
    fputs(server_message, file);
    fclose(file);

    close(socket_desc);
}

/**
 * Places a file on the remote server
 * @param source file in local
 * @param destination file to be created on server
 * @param socket_desc socket to use
 */
void handle_put(char source[], char destination[], int socket_desc) {
    printf("Source: %s\nDestination: %s\n", source, destination);
    //something to read lines in file
    char lineContent[121];
    //the file
    FILE* file = fopen(source, "r");
    //something to hold the entire file content
    char client_message[9104];
    //command id
    strcat(client_message, "p ");
    //location of new file
    strcat(client_message, destination);
    //space delimiter
    strcat(client_message, "\n");
    //get contents from file and add them to the client message
    while(fgets(lineContent, 120, file)){
        strcat(client_message, lineContent);
    }
    fclose(file);

    //send it bro
    if(send(socket_desc, client_message, strlen(client_message), 0) < 0){
        printf("Unable to send message\n");
    }
    //something to hold the response
    char server_message[2001];
    memset(server_message, '\0', sizeof(server_message));
    //bring it bro
    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
        printf("Error while receiving server's msg\n");
    }
    //print it bro
    printf("Server's response: %s\n", server_message);
    close(socket_desc);
}

/**
 * Lists file information
 * @param fileName file for which info is requested
 * @param socket_desc socket to use
 */
void handle_info(char fileName[], int socket_desc) {
    char command[103] = "i ";
    strcat(command, fileName);
    if (send(socket_desc, command, strlen(command), 0) < 0) {
        printf("Unable to send message\n");
    }

    char server_message[2001];
    memset(server_message, '\0', sizeof(server_message));
    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
        printf("Error while receiving server's msg\n");
    }
    printf("Server's response: \n%s", server_message);
    close(socket_desc);
}

/**
 * Creates a new directory on server
 * @param fileName the directory to be created
 * @param socket_desc socket to use
 */
void handle_md(char fileName[], int socket_desc) {
    char command[103] = "m ";
    strcat(command, fileName);
    if (send(socket_desc, command, strlen(command), 0) < 0) {
        printf("Unable to send message\n");
    }

    char server_message[2001];
    memset(server_message, '\0', sizeof(server_message));
    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
        printf("Error while receiving server's msg\n");
    }
    printf("Server's response: %s\n", server_message);
    close(socket_desc);
}

/**
 * Deletes a file from the server
 * @param fileName file to be deleted
 * @param socket_desc socket to use
 */
void handle_rm(char fileName[], int socket_desc) {
    char command[103] = "r ";
    strcat(command, fileName);
    if (send(socket_desc, command, strlen(command), 0) < 0) {
        printf("Unable to send message\n");
    }

    char server_message[2001];
    memset(server_message, '\0', sizeof(server_message));
    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
        printf("Error while receiving server's msg\n");
    }
    printf("Server's response: %s\n", server_message);
    close(socket_desc);
}

/**
 * Shuts down the server
 * @param socket_desc socket to use
 */
void handle_exit(int socket_desc){
    char command[] = "e";
    if (send(socket_desc, command, strlen(command), 0) < 0) {
        printf("Unable to send message\n");
    }

    char server_message[2001];
    memset(server_message, '\0', sizeof(server_message));

    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
        printf("Error while receiving server's msg\n");
    }
    printf("Server's response: %s\n", server_message);
}

int main(int argc, char *argv[]) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0) {
        printf("Unable to create socket\n");
        return -1;
    }

    printf("Socket created successfully\n");

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send connection request to server:
    if (connect(socket_desc, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");

    if (strcmp(argv[1], "GET") == 0) {
        if(argc == 3){
            handle_get(argv[2], argv[2], socket_desc);
        } else {
            handle_get(argv[2], argv[3], socket_desc);
        }
    } else if (strcmp(argv[1], "PUT") == 0) {
        if(argc == 3){
            handle_put(argv[2], argv[2], socket_desc);
        } else {
            handle_put(argv[2], argv[3], socket_desc);
        }
    } else if (strcmp(argv[1], "INFO") == 0) {
        handle_info(argv[2], socket_desc);
    } else if (strcmp(argv[1], "MD") == 0) {
        handle_md(argv[2], socket_desc);
    } else if (strcmp(argv[1], "RM") == 0) {
        handle_rm(argv[2], socket_desc);
    } else if (strcmp(argv[1], "EXIT") == 0) {
        handle_exit(socket_desc);
    } else {
        printf("Invalid command: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
