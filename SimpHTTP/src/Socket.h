#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>

class Socket
{
public:
	Socket(const char* iNode, const char* iPort);
	~Socket();

	void Listen(int backlog);
	std::string Recv(int bytesToRecv);
	void Accept();
	void Send(std::string sendString);
	void Shutdown();
private:
	WSADATA wsaData;
	SOCKET listenSocket, clientSocket = INVALID_SOCKET;
	struct addrinfo* result = nullptr;
	struct addrinfo hints;
};
