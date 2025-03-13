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

void ExitWithError(const char *errorMessage) {
    #ifdef _WIN32
    printf("Usao je u error\n");
    printf("Error: %d\n", WSAGetLastError());
    WSACleanup();
    #else
    perror(errorMessage);
    #endif
    exit(EXIT_FAILURE);
}

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

    char value[6];
    switch (card->value)
    {
        case 1:
            strncpy(value, "ace", 4);
            break;
        case 11:
            strncpy(value, "jack", 5);
            break;
        case 12:
            strncpy(value, "queen", 6);
            break;
        case 13:
            strncpy(value, "king", 5);
            break;
        default:
            sprintf(value, "%d", card->value);
            break;
    }

    char *card_name;
    card_name = malloc(sizeof(char) * 50);
    sprintf(card_name, "%s_of_%s", value, suit);
    printf("%s\n", card_name);
    return card_name;
}

int main() {

    #ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET socket_id = socket(AF_INET, SOCK_STREAM, 0);
    #else
    int socket_id = socket(AF_INET, SOCK_STREAM, 0);
    #endif

    if (socket_id == -1) {
        ExitWithError("socket() failed");
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);

    if (bind(socket_id, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        ExitWithError("bind() failed");
    }

    if (listen(socket_id, 3) < 0) {
        ExitWithError("listen() failed");
    }

    struct sockaddr_in client_address;
    int client_address_length = sizeof(client_address);
    int new_socket_id = accept(socket_id, (struct sockaddr *)&client_address, (socklen_t *)&client_address_length);

    if (new_socket_id < 0) {
        ExitWithError("accept() failed");
    }

    char buffer[MAX_BUFFER_SIZE] = {0};
    int msgbyte = recv(new_socket_id, buffer, sizeof(CARD), 0);
    if (msgbyte == -1) {
        ExitWithError("recv() failed");
    }
    CARD card_buffer = *(CARD *)buffer;
    // printf("%d\n", card_buffer.value);
    // printf("%d\n", card_buffer.suit);

    char *card_name = full_card_name(&card_buffer);
    printf("radi\n");
    printf("Received card: %s\n", card_name);

    free(card_name);

    #ifdef _WIN32
    WSACleanup();
    closesocket(socket_id);
    #else
    close(socket_id);
    #endif
}