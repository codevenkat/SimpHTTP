#pragma once

#ifdef WIN32 // windows
#include <WinSock2.h>
#include <WS2tcpip.h>

using socket_t = SOCKET;
#else // POSIX
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

using socket_t = int;
constexpr int INVALID_SOCKET = -1;
constexpr int SOCKET_ERROR = -1;
#endif

#include <iostream>
#include <string>

class Socket
{
public:
	Socket();
	Socket(socket_t sock);
	~Socket();

	void Bind(const char* iNode, const char* iPort);
	void Listen(int backlog);
	std::string Recv(int bytesToRecv);
	Socket Accept();
	void Send(const std::string& sendString);
	void CloseSocket();
	void Shutdown();

private:
	socket_t m_Sockfd = INVALID_SOCKET;
	int iResult;

	void LogError(std::string msg);
	void LogInfo(std::string msg);
};
