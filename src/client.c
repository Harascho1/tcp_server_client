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

void ExitWithError(const char* errorMessage) {
    #ifdef _WIN32
    printf("Usao je u error\n");
    printf("Error: %d\n", WSAGetLastError());
    WSACleanup();
    #else
    perror(errorMessage);
    #endif
    exit(EXIT_FAILURE);
}

int main() {

    #ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        ExitWithError();
    }
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
    int c = connect(socket_id, (struct sockaddr *)&server_address, sizeof(server_address));
    if (c == -1) {
        ExitWithError("connect() failed");
    }
    printf("radi\n");


    // Hocu serveru da posaljem KEC PIK
    CARD card;
    card.value = value_ace;
    card.suit = suit_spades;
    card.selected = 0;


    int msg_byte = send(socket_id, (char*)&card, sizeof(CARD), 0);
    if (msg_byte == -1) {
        ExitWithError("send() failed");
    }


    #ifdef _WIN32
    WSACleanup();
    closesocket(socket_id);
    #else
    close(socket_id);
    #endif

}