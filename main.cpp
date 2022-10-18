/* **CLIENT** */

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


int main() {

    WSADATA wsaData;
    ADDRINFO hints;
    ADDRINFO *addrResult = NULL;
    SOCKET connectSocket = INVALID_SOCKET;

    const char *sendBuffer = "Hello from client number 1!";
    char recvBuffer[512];

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStart failed, result = " << result << std::endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    result = getaddrinfo("localhost", "666", &hints, &addrResult);
    if (result != 0) {
        std::cout << "getaddrinfo failed, result = " << result << std::endl;
        WSACleanup();
        return 1;
    }

    connectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (connectSocket == INVALID_SOCKET) {
        std::cout << "connectSocket creation failed" << std::endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = connect(connectSocket, addrResult->ai_addr, (int) addrResult->ai_addrlen);
    if (result == SOCKET_ERROR) {
        std::cout << "Unable connect to server" << std::endl;
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    std::string sendStr;
    while(sendStr != "quit"){
        std::cout<< "Me: ";
        _flushall();
        getline(std::cin, sendStr);
        result = send(connectSocket, sendStr.data(), (int) sendStr.length(), 0);
        if (result == SOCKET_ERROR) {
            std::cout << "send failed, error:" << std::endl;
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            freeaddrinfo(addrResult);
            WSACleanup();
            return 1;
        }
        ZeroMemory(recvBuffer, 512);
        result = recv(connectSocket, recvBuffer, 512, 0);
        if (result > 0) {
            std::cout << "Server: " << recvBuffer << std::endl;
        } else if (result == 0) {
            std::cout << "Connection close " << std::endl;
        } else {
            std::cout << "recv failed with error " << std::endl;
        }
    }
    result = shutdown(connectSocket,SD_SEND);
    if(result == SOCKET_ERROR){
        std::cout << "shutdown failed, error:" << std::endl;
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    closesocket(connectSocket);
    connectSocket = INVALID_SOCKET;
    freeaddrinfo(addrResult);
    WSACleanup();
    return 0;
}
