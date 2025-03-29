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
        ExitWithError("WSAStartup() failed");
    }
    SOCKET socket_id = socket(AF_INET, SOCK_STREAM, 0);
    #else
    int socket_id = socket(AF_INET, SOCK_STREAM, 0);
    #endif
    if (socket_id == -1) {
        ExitWithError("socket() failed");
    }

    printf("WTF\n");
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "178.220.18.75", &server_address.sin_addr) <= 0) {
        ExitWithError("inet_pton() failed");
    }
    printf("Povezivanje na server...\n");
    int c = connect(socket_id, (struct sockaddr *)&server_address, sizeof(server_address));
    if (c == -1) {
        ExitWithError("connect() failed");
    }
    printf("Povezan na server\n");

    CARD card = {10, suit_spades, 0};

    int msg_byte = send(socket_id, (char*)&card, sizeof(CARD), 0);
    if (msg_byte == -1) {
        ExitWithError("send() failed");
    }

    #ifdef _WIN32
    closesocket(socket_id);
    WSACleanup();
    #else
    close(socket_id);
    #endif

}