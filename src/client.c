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

    int c = connect(socket_id, (struct sockaddr *)&server_address, sizeof(server_address));
    if (c != 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }


    // Hocu serveru da posaljem KEC PIK
    CARD card;
    card.value = value_ace;
    card.suit = suit_spades;
    card.selected = 0;

    int msg_byte = send(socket_id, &card, sizeof(CARD), 0);
    if (msg_byte == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }


    #ifdef _WIN32
    WSACleanup();
    closesocket(socket_id);
    #else
    close(socket_id);
    #endif

}