/*
 * server.c -- TCP Socket Server
 * 
 * adapted from: 
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * String splicer to extract substrings
 * @param str starting string
 * @param result desired string
 * @param start starting location of splice
 * @param end ending location of splice
 */
void stringSplice(char *str, char *result, int start, int end){
    strncpy(result, str + start, end - start);
}

/**
 * Find the requested file and send it back
 * @param client_message details of the desire file
 * @param client_sock_desc socket to use
 */
void handle_get(char client_message[], int client_sock_desc){
    char serverPath[1000];
    FILE* servLocFile = fopen("serverLocation.txt", "r");
    fgets(serverPath, 1000, servLocFile);
    fclose(servLocFile);

    char fileName[101];
    char lineContent[121];
    char c[2];
    sscanf(client_message, "%c %s", c, fileName);
    printf("fileName: %s\n", fileName);
    strcat(serverPath, fileName);
    printf("serverPath: %s\n", serverPath);

    FILE* file = fopen(serverPath, "r");
    char server_message[8196];
    memset(server_message, '\0', sizeof(server_message));
    while (fgets(lineContent, 120,file)){
        strcat(server_message, lineContent);
    }
    fclose(file);
    if (send(client_sock_desc, server_message, strlen(server_message), 0) < 0) {
        printf("Can't send\n");
    }
}

/**
 * Take the information received and place it in a file.
 * @param client_message details of the file to create
 * @param client_sock_desc socket to use
 */
void handle_put(char client_message[], int client_sock_desc) {
    char serverPath[1000];
    FILE* servLocFile = fopen("serverLocation.txt", "r");
    fgets(serverPath, 1000, servLocFile);
    fclose(servLocFile);

    char c[2];
    char fileName[101];
    char lineContent[121];
    char fileContent[9001];
    sscanf(client_message, "%s %s", c, fileName);
    strcat(serverPath, fileName);
    int startOfMessage = -1;
    for(int i = 0; i < strlen(client_message); i++){
        if (client_message[i] == '\n'){
            startOfMessage = i+1;
            break;
        }
    }
    stringSplice(client_message, fileContent, startOfMessage, strlen(client_message));
    printf("fileContent: %s\n", fileContent);
    FILE* file = fopen(serverPath, "w");
    printf("Creating file...\n");
    printf("Writing file...\n");
    fputs(fileContent, file);
    fclose(file);

    char server_message[] = "File received and saved.";
    if (send(client_sock_desc, server_message, strlen(server_message), 0) < 0) {
        printf("Can't send\n");
    }
}

/**
 * Retreive file details
 * @param client_message desired file
 * @param client_sock_desc socket to use
 */
void handle_info(char client_message[], int client_sock_desc) {
    char serverPath[1000];
    FILE* servLocFile = fopen("serverLocation.txt", "r");
    fgets(serverPath, 1000, servLocFile);
    fclose(servLocFile);

    char c[2];
    char filename[101];
    sscanf(client_message, "%s %s", c, filename);
    strcat(serverPath, filename);
    struct stat fileInfo;
    stat(serverPath, &fileInfo);
    char uid[20];
    snprintf(uid, 20, "uid:  %d\n", (int)fileInfo.st_uid);
    char gid[20];
    snprintf(gid, 20, "gid:  %d\n", (int)fileInfo.st_gid);
    char mode[30];
    strcat(mode, "mode: ");
    strcat(mode, ((fileInfo.st_mode & S_IRUSR) ? "r" : "-"));
    strcat(mode, ((fileInfo.st_mode & S_IWUSR) ? "w" : "-"));
    strcat(mode, ((fileInfo.st_mode & S_IXUSR) ? "x" : "-"));
    strcat(mode, ((fileInfo.st_mode & S_IRGRP) ? "r" : "-") );
    strcat(mode, ((fileInfo.st_mode & S_IWGRP) ? "w" : "-") );
    strcat(mode, ((fileInfo.st_mode & S_IXGRP) ? "x" : "-") );
    strcat(mode, ((fileInfo.st_mode & S_IROTH) ? "r" : "-") );
    strcat(mode, ((fileInfo.st_mode & S_IWOTH) ? "w" : "-") );
    strcat(mode, ((fileInfo.st_mode & S_IXOTH) ? "x\n" : "-\n"));
    char server_message[8196];
    memset(server_message, '\0', sizeof(server_message));
    strcat(server_message, uid);
    strcat(server_message, gid);
    strcat(server_message, mode);

    if (send(client_sock_desc, server_message, strlen(server_message), 0) < 0) {
        printf("Can't send\n");
    }
}

/**
 * make a new directory
 * @param client_message name of new directory
 * @param client_sock_desc socket to use
 */
void handle_md(char client_message[], int client_sock_desc) {
    char serverPath[1000];
    FILE* servLocFile = fopen("serverLocation.txt", "r");
    fgets(serverPath, 1000, servLocFile);
    fclose(servLocFile);

    char c[2];
    char directory[101];
    sscanf(client_message, "%s %s", c, directory);
    strcat(serverPath, directory);
    int status = mkdir(serverPath, ACCESSPERMS);
    chmod(serverPath, ACCESSPERMS);
    char server_message[100];
    memset(server_message, '\0', sizeof(server_message));
    if (status == 0){
        strcpy(server_message, "STATUS 0: Directory created.");
    }
    else{
        strcpy(server_message, "STATUS -1: Error in making directory.");
    }
    if (send(client_sock_desc, server_message, strlen(server_message), 0) < 0) {
        printf("Can't send\n");
    }
}

/**
 * delete a file
 * @param client_message file to be deleted
 * @param client_sock_desc socket to use
 */
void handle_rm(char client_message[], int client_sock_desc) {
    char serverPath[1000];
    FILE* servLocFile = fopen("serverLocation.txt", "r");
    fgets(serverPath, 1000, servLocFile);
    fclose(servLocFile);

    char c[2];
    char filename[101];
    sscanf(client_message, "%s %s", c, filename);
    strcat(serverPath, filename);
    int status = remove(serverPath);
    char server_message[100];
    memset(server_message, '\0', sizeof(server_message));
    if (status == 0){
        strcpy(server_message, "STATUS 0: File removed.");
    }
    else{
        strcpy(server_message, "STATUS -1: Error in removing file.");
    }
    if (send(client_sock_desc, server_message, strlen(server_message), 0) < 0) {
        printf("Can't send\n");
    }
}

int main(void) {
    int socket_desc, client_sock_desc;
    socklen_t client_size;
    struct sockaddr_in server_addr, client_addr;
    int exitFlag = 0;


    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind to the set port and IP:
    if (bind(socket_desc, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");

    while (exitFlag != 1){
        // Listen for clients:
        char server_message[8196];
        memset(server_message, '\0', sizeof(server_message));
        if (listen(socket_desc, 1) < 0) {
            printf("Error while listening\n");
            return -1;
        }
        printf("\nListening for incoming connections.....\n");

        // Accept an incoming connection:
        client_size = sizeof(client_addr);
        client_sock_desc = accept(socket_desc, (struct sockaddr *) &client_addr, &client_size);

        if (client_sock_desc < 0) {
            printf("Can't accept\n");
            return -1;
        }
        printf("Client connected at IP: %s and port: %i\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        char client_message[8196];
        memset(client_message, '\0', sizeof(client_message));
        // Receive client's message:
        if (recv(client_sock_desc, client_message, sizeof(client_message), 0) < 0) {
            printf("Couldn't receive\n");
            return -1;
        }
        printf("Command from client: %c\n", client_message[0]);
        if (strncmp(client_message, "g", 1) == 0){
            handle_get(client_message, client_sock_desc);
        }
        else if(strncmp(client_message, "p", 1) == 0){
            handle_put(client_message, client_sock_desc);
        }
        else if(strncmp(client_message, "i", 1) == 0){
            handle_info(client_message, client_sock_desc);
        }
        else if(strncmp(client_message, "m", 1) == 0){
            handle_md(client_message, client_sock_desc);
        }
        else if(strncmp(client_message, "r", 1) == 0){
            handle_rm(client_message, client_sock_desc);
        }
        else if(strncmp(client_message, "e", 1) == 0){
            exitFlag = 1;
            char server_message[] = "Shutting down server.";
            if (send(client_sock_desc, server_message, strlen(server_message), 0) < 0) {
                printf("Can't send\n");
            }
            close(socket_desc);
            break;
        }
    }

    return 0;
}
