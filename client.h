//
// Created by Kashyap Tare on 4/17/23.
//

#ifndef PRACTICUM2_CLIENT_H
#define PRACTICUM2_CLIENT_H
void handle_get(char source[], char destination[], int socket_desc);
void handle_put(char source[], char destination[], int socket_desc);
void handle_info(char fileName[], int socket_desc);
void handle_md(char fileName[], int socket_desc);
void handle_rm(char fileName[], int socket_desc);
void handle_exit(int socket_desc);
#endif //PRACTICUM2_CLIENT_H
