#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "card.h"

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

char* full_card_name(CARD *card) {
    char suit[10];
    switch (card->suit) {
        case suit_clubs:
            strncpy(suit, "clubs", 6);
            break;
        case suit_diamonds:
            strncpy(suit, "diamonds", 9);
            break;
        case suit_hearts:
            strncpy(suit, "hearts", 7);
            break;
        case suit_spades:
            strncpy(suit, "spades", 7);
            break;
        default:
            printf("Unknown suit");
            fflush(stdout);
            return NULL;
    }

    char value[5];
    switch (card->value)
    {
        case 1:
            strncpy(value, "ace", 3);
            break;
        case 11:
            strncpy(value, "jack", 4);
            break;
        case 12:
            strncpy(value, "queen", 5);
            break;
        case 13:
            strncpy(value, "king", 4);
            break;
        default:
            sprintf(value, "%d", card->value);
            break;
    }

    char *card_name;
    card_name = malloc(sizeof(char) * 50);
    sprintf(card_name, "%s_of_%s", value, suit);
    return card_name;
}

int main() {

    #ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET socket_id = socket(AF_INET, SOCK_STREAM, 0);
    #else
    int socket_id = socket(AF_INET, SOCK_STREAM, 0);


    if (socket_id == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    #endif

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(socket_id, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(socket_id, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_address;
    int client_address_length = sizeof(client_address);
    int new_socket_id = accept(socket_id, (struct sockaddr *)&client_address, (socklen_t *)&client_address_length);

    if (new_socket_id < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    CARD card_buffer;
    int msgbyte = recv(new_socket_id, &card_buffer, sizeof(CARD), 0);
    if (msgbyte == -1) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    char *card_name = full_card_name(&card_buffer);
    printf("Received card: %s\n", card_name);


    #ifdef _WIN32
    WSACleanup();
    closesocket(socket_id);
    #else
    close(socket_id);
    #endif
}