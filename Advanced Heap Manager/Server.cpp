#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_PORT "27016"
#define BUFFER_SIZE 512
#define MAX_CLIENTS 5


typedef struct ClientData {
	SOCKET clientSocket;
	int clientIndex;
} ClientData;


bool clientSlots[MAX_CLIENTS] = { false }; //BOOL is either TRUE (used) or FALSE (free)
ClientData* clientData[MAX_CLIENTS] = { NULL }; //clientData holds the client information

void printClientStatus() {
	int connectedClients = 0;
	printf("\n=====================================================================\n");
	printf("\t\t\tCurrent Client Slots Status");
	printf("\n=====================================================================\n");
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clientSlots[i]) { // Slot is occupied
			printf("\tSlot %d: Occupied by Client %d (Socket %d)\n", i + 1, clientData[i]->clientIndex + 1, clientData[i]->clientSocket);

			connectedClients++;
		}
		else { // Slot is free
			printf("\tSlot %d: Free\n", i + 1);
		}
	}

	printf("_____________________________________________________________________\n");
	printf("\tTotal connected clients: %d", connectedClients);
	printf("\n=====================================================================\n");
}

void printNewConnection(SOCKET clientSocket, int slotFound) {
	printf("\n=====================================================================\n");
	printf("\t\t\tNEW CLIENT CONNECTION");
	printf("\n=====================================================================\n");
	printf("\tSocket: %d\n", clientSocket);
	printf("\tAssigned to Slot: %d", slotFound + 1);
	printf("\n=====================================================================\n\n");


}

void printClientDisconnect(int clientIndex, int reason) {
	printf("\n=====================================================================\n");
	printf("\t\t\tCLIENT DISCONNECT REQUEST");
	printf("\n=====================================================================\n");

	switch (reason) {
	case 0:
		// Client explicitly requested to exit
		printf("\tClient %d has requested to exit.\n", clientIndex + 1);
		printf("\tConnection with client %d closed.", clientIndex + 1);
		break;

	case 1:
		// Failed to receive data
		printf("\tClient %d lost connection or failed to send data.\n", clientIndex + 1);
		printf("\trecv failed with error: %d", WSAGetLastError());
		break;

	case 2:
		// Timeout or other disconnection reason
		printf("\tClient %d has disconnected due to timeout or other error.", clientIndex + 1);
		break;

	default:
		printf("\tClient %d disconnected for an unknown reason.", clientIndex + 1);
		break;
	}
	printf("\n=====================================================================\n");


}

void handleClient(void* clientData) {
	ClientData* data = (ClientData*)clientData;
	SOCKET clientSocket = data->clientSocket;
	int clientIndex = data->clientIndex;

	// Buffer used for storing the choosen option
	char recvBuffer[BUFFER_SIZE];

	// Buffer used for storing incoming data
	char dataBuffer[BUFFER_SIZE];

	// Buffer used response message
	char sendBuffer[BUFFER_SIZE];

	int iResult;

	const char* welcomeMessage =
		"Welcome to the server. \n";

	const char* menuMessage =
		"Choose a menu option:\n"
		"\t1. Allocate memory\n"
		"\t2. Free memory\n"
		"\t0. Exit\n";

	send(clientSocket, welcomeMessage, (int)strlen(welcomeMessage), 0);

	while (1) {

		send(clientSocket, menuMessage, (int)strlen(menuMessage), 0);
		iResult = recv(clientSocket, recvBuffer, BUFFER_SIZE, 0);

		if (iResult > 0) {
			recvBuffer[iResult] = '\0';

			char optionstr[BUFFER_SIZE];
			char message[BUFFER_SIZE];

			if (sscanf_s(recvBuffer, "%[^|]|%s", optionstr, sizeof(optionstr), message, sizeof(message)) == 2)
			{
				int option = atoi(optionstr);
				switch (option)
				{
				case 1: {
					printf("\n=====================================================================\n");
					printf("\t\t\tMEMORY ALLOCATION REQUEST");
					printf("\n=====================================================================\n");

						message[iResult] = '\0';
						int numBytes = atoi(message);
						printf("\tClient %d has requested memory allocation\n", clientIndex + 1);
						printf("\tRequested Memory Size: %d bytes", numBytes);

						printf("\n=====================================================================\n");

						//TO DO

						sprintf_s(sendBuffer, "Allocating %d bytes of memory...", numBytes);
						send(clientSocket, sendBuffer, (int)strlen(sendBuffer), 0);

						//
					break;
				}
				case 2: {
					printf("\n=====================================================================\n");
					printf("\t\tGET OCCUPIED MEMORY LOCATIONS REQUEST");
					printf("\n=====================================================================\n");

					printf("\tClient %d has requested to free memory\n", clientIndex + 1);
					printf("\tSending occupied memory locations...");

					//TO DO
					const char* occupiedMemoryLocations = "spisak zauzetih memorijskih lokacija....";
					//

					send(clientSocket, occupiedMemoryLocations, (int)strlen(occupiedMemoryLocations), 0);

					printf("\n=====================================================================\n");


					iResult = recv(clientSocket, recvBuffer, BUFFER_SIZE, 0);

					printf("\n=====================================================================\n");
					printf("\t\t\tFREE MEMORY REQUEST");
					printf("\n=====================================================================\n");

					if (iResult > 0) {
						recvBuffer[iResult] = '\0';
						printf("\tClient %d has requested to free memory on %s location\n", clientIndex + 1, recvBuffer);
						
						printf("\n=====================================================================\n");

						//TO DO
					}
					else {
						printf("\tFailed to receive memory size or free memory request from client %d", clientIndex + 1);
						printf("\n=====================================================================\n");
					}

					break;
				}
				case 0: {
					printClientDisconnect(clientIndex, 0);
					closesocket(clientSocket);
					free(data);
					clientSlots[clientIndex] = FALSE; // Slot is now free
					printClientStatus();
					_endthread();
					return;
				}
				default:
					const char* invalidMessage = "Invalid option. Please try again.";
					send(clientSocket, invalidMessage, (int)strlen(invalidMessage), 0);
					break;
				}

			}
			

		}
		else if(iResult == 0)
		{
			printClientDisconnect(clientIndex, 0);
			closesocket(clientSocket);
			free(data);
			clientSlots[clientIndex] = FALSE; // Slot is now free
			printClientStatus();
			_endthread();
			return;
			 
		}
		else
		{
			printClientDisconnect(clientIndex, 1);
			closesocket(clientSocket);
			free(data);
			clientSlots[clientIndex] = FALSE; // Slot is now free
			printClientStatus();
			_endthread();
			return;
		}
	}
}


int main()
{
	// WSADATA data structure that is to receive details of the Windows Sockets implementation
	WSADATA wsaData;

	// Variable used to store function return value
	int iResult;

	// Socket used for listening for new clients 
	SOCKET listenSocket = INVALID_SOCKET;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;					// IPv4 address family
	hints.ai_socktype = SOCK_STREAM;			// Stream socket
	hints.ai_protocol = IPPROTO_TCP;			// TCP protocol
	hints.ai_flags = AI_PASSIVE;				// Allow the server to bind to any address

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a socket for connecting to the server
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	// Check if socket is successfully created
	if (listenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}


	// Set listenSocket in listening mode
	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	// Set the socket to listen mode
	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	printf("Server is listening on port %s\n", DEFAULT_PORT);

	while (1)
	{
		SOCKET clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			continue;
		}

		int slotFound = -1;
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (!clientSlots[i]) {
				slotFound = i;
				clientSlots[i] = TRUE;
				break;
			}
		}

		if (slotFound == -1) {
			printf("Max clients reached. Rejecting new connection.\n");
			closesocket(clientSocket);
		}
		else {
			printNewConnection(clientSocket, slotFound);
			clientData[slotFound] = (ClientData*)malloc(sizeof(ClientData));
			clientData[slotFound]->clientSocket = clientSocket;
			clientData[slotFound]->clientIndex = slotFound;

			printClientStatus();

			_beginthread(handleClient, 0, (void*)clientData[slotFound]);
		}
	}


	//Close listen and accepted sockets
	closesocket(listenSocket);

	// Deinitialize WSA library
	WSACleanup();

	return 0;
    
}


