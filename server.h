//
// Created by Kashyap Tare on 4/17/23.
//

#ifndef PRACTICUM2_SERVER_H
#define PRACTICUM2_SERVER_H
void stringSplice(char *str, char *result, int start, int end);
void handle_get(char client_message[], int client_sock_desc);
void handle_put(char client_message[], int client_sock_desc);
void handle_info(char client_message[], int client_sock_desc);
void handle_md(char client_message[], int client_sock_desc);
void handle_rm(char client_message[], int client_sock_desc);
#endif //PRACTICUM2_SERVER_H
