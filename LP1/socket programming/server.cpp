#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

// Vector to store all connected client sockets
std::vector<SOCKET> clients;
CRITICAL_SECTION clientsLock;

// Thread function for a single client
DWORD WINAPI handle_client(LPVOID clientSocket) {
    SOCKET sock = (SOCKET)clientSocket;
    char buffer[1024];
    int bytesReceived;

    while (true) {
        bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break; // client disconnected
        buffer[bytesReceived] = '\0';

        std::cout << "Received: " << buffer << std::endl;

        // Broadcast to all clients
        EnterCriticalSection(&clientsLock);
        for (SOCKET s : clients) {
            if (s != sock) { // optional: don't send back to sender
                send(s, buffer, bytesReceived, 0);
            }
        }
        LeaveCriticalSection(&clientsLock);
    }

    // Remove client from vector
    EnterCriticalSection(&clientsLock);
    clients.erase(std::remove(clients.begin(), clients.end(), sock), clients.end());
    LeaveCriticalSection(&clientsLock);

    closesocket(sock);
    return 0;
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    InitializeCriticalSection(&clientsLock);

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "[*] Multi-threaded Broadcast Server listening on 127.0.0.1:8080\n";

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) continue;

        char* clientIP = inet_ntoa(clientAddr.sin_addr);
        std::cout << "[*] Client connected: " << clientIP
                  << ":" << ntohs(clientAddr.sin_port) << "\n";

        // Add client to vector
        EnterCriticalSection(&clientsLock);
        clients.push_back(clientSocket);
        LeaveCriticalSection(&clientsLock);

        // Start thread
        HANDLE hThread = CreateThread(NULL, 0, handle_client, (LPVOID)clientSocket, 0, NULL);
        if (hThread) CloseHandle(hThread);
    }

    DeleteCriticalSection(&clientsLock);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
