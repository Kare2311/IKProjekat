#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma comment (lib, "Ws2_32.lib")

#pragma pack(1)

#define SERVER_IP_ADDRESS "127.0.0.1"
#define DEFAULT_PORT "27016"
#define BUFFER_SIZE 512

int main()
{
    // WSADATA data structure that is to receive details of the Windows Sockets implementation
    WSADATA wsaData;

    // Socket used to communicate with server
    SOCKET connectSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL, * ptr = NULL, hints;

    // Variable used to store function return value
    int iResult;

    // Buffer we will use to store message
    char recvBuffer[BUFFER_SIZE];

    int recvBufferLen = BUFFER_SIZE;

    // Initialize windows sockets library for this process
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;                  // IPv4 protocol
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(SERVER_IP_ADDRESS, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) 
    {
        // Create a socket for connecting to server
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server
        iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (connectSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");

    // Receive welcome message
    iResult = recv(connectSocket, recvBuffer, recvBufferLen, 0);
    if (iResult > 0) {
        recvBuffer[iResult] = '\0';
        printf("%s\n", recvBuffer);
    }


    while (1)
    {
        // Receive menu message
        iResult = recv(connectSocket, recvBuffer, recvBufferLen, 0);
        if (iResult > 0) {
            recvBuffer[iResult] = '\0';
            printf("%s\n", recvBuffer);
        }

        char sendBuffer[BUFFER_SIZE];
        printf("Enter option: ");

        fgets(sendBuffer, BUFFER_SIZE, stdin);
        sendBuffer[strcspn(sendBuffer, "\n")] = 0; // Remove newline character

        int option = atoi(sendBuffer);

        if (option == 0) {
            snprintf(sendBuffer, BUFFER_SIZE, "0|0");
            iResult = send(connectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
            break;
        }

        // Handle option-specific communication
        if (option == 1) {
            // Allocate memory
            char sizebuf[BUFFER_SIZE];
            printf("Enter the number of bytes to allocate: ");
            fgets(sizebuf, BUFFER_SIZE, stdin);
            sizebuf[strcspn(sizebuf, "\n")] = 0; // Remove newline character

            snprintf(sendBuffer, BUFFER_SIZE, "1|%s", sizebuf);

            // Send option and memory size to server
            iResult = send(connectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(connectSocket);
                WSACleanup();
                return 1;
            }

            iResult = recv(connectSocket, recvBuffer, recvBufferLen, 0);
            if (iResult > 0) {
                recvBuffer[iResult] = '\0';
                printf("%s\n", recvBuffer);
            }
        }

        if (option == 2) {
            // Free memory
            char sizebuf[BUFFER_SIZE]; // Remove newline character

            snprintf(sendBuffer, BUFFER_SIZE, "2|0");

            // Send option to server
            iResult = send(connectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(connectSocket);
                WSACleanup();
                return 1;
            }

            //ispis zauzetih adresa u memoriji
            iResult = recv(connectSocket, recvBuffer, recvBufferLen, 0);
            if (iResult > 0) {
                recvBuffer[iResult] = '\0';
                printf("%s\n", recvBuffer);
            }

            printf("Choose an address where you want to free memory: ");
            fgets(sendBuffer, BUFFER_SIZE, stdin);
            sendBuffer[strcspn(sendBuffer, "\n")] = 0;

            iResult = send(connectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(connectSocket);
                WSACleanup();
                return 1;
            }

        }

        
    }

    // Close connected socket
    closesocket(connectSocket);

    // Deinitialize WSA library
    WSACleanup();

    return 0;
}
