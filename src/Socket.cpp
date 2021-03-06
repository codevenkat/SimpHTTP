#include "Socket.h"

// some specific code for winsock and BSD sockets
static void closesocket_(socket_t sockToClose)
{
	// closesocket() on winodws, close() on POSIX
#ifdef WIN32
	closesocket(sockToClose);
#else
	close(sockToClose);
#endif
}

static void WSACleanup_()
{
#ifdef WIN32
	WSACleanup();
#endif
}

int Socket::GetLastError()
{
#ifdef WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

// inits socket(windows)
Socket::Socket()
{
#ifdef WIN32
	WSADATA wsaData;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		spdlog::error("Failed WSAStartup");
		WSACleanup_();
		exit(1);
	}
#endif
}

Socket::Socket(socket_t sockfd)
{
#ifdef WIN32
	WSADATA wsaData;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		spdlog::error("Failed WSAStartup: {}", WSAGetLastError());
		WSACleanup_();
		exit(1);
	}
#endif

	m_Sockfd = sockfd;
}

void Socket::Bind(const char* iNode, const char* iPort)
{
	struct addrinfo* result = nullptr;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(iNode, iPort, &hints, &result);
	if (iResult != 0)
	{
		spdlog::error("getaddrinfo failed: {}", gai_strerror(iResult));
		freeaddrinfo(result);
		WSACleanup_();
		exit(1);
	}

	// create listening socket
	m_Sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_Sockfd == INVALID_SOCKET)
	{
		spdlog::error("socket failed. Error: {}", GetLastError());
		freeaddrinfo(result);
		WSACleanup_();
		exit(1);
	}

	// bind socket
	iResult = bind(m_Sockfd, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		spdlog::error("bind failed. Error: {}", GetLastError());
		freeaddrinfo(result);
		WSACleanup_();
		exit(1);
	}

	freeaddrinfo(result); // finished binding
}

Socket::~Socket()
{
}

void Socket::Listen(int backlog)
{
	iResult = listen(m_Sockfd, backlog);
	if (iResult != 0)
	{
		spdlog::error("listen failed. Error: {}", GetLastError());
		closesocket_(m_Sockfd);
		WSACleanup_();
		exit(1);
	}
}

Socket Socket::Accept()
{
	socket_t clientSocket = INVALID_SOCKET;
	clientSocket = accept(m_Sockfd, NULL, NULL);
	if (clientSocket == INVALID_SOCKET)
	{
		spdlog::error("accept failed. Error: {}", GetLastError());
		closesocket_(m_Sockfd);
		WSACleanup_();
		exit(1);
	}

	return Socket(clientSocket);
}

std::string Socket::Recv(int bytesToRecv)
{
	// iResult -> no. of bytes recv'd
	std::string recvData;
	recvData.resize(bytesToRecv);

	iResult = recv(m_Sockfd, recvData.data(), bytesToRecv, 0);

	if (iResult > 0)
	{
		spdlog::info("Received bytes: {}", iResult);
	}
	else if (iResult == 0)
		spdlog::info("Closing connection...");
	else
	{
		// failed
		spdlog::error("recv failed. Error: {}", GetLastError());
		closesocket_(m_Sockfd);
		WSACleanup_();
		exit(1);
	}

	return recvData;
}

void Socket::Send(const std::string& sendString)
{
	int iResult;

	iResult = send(m_Sockfd, sendString.c_str(), sendString.length(), 0);
	spdlog::info("Bytes sent: {}", iResult);
	if (iResult == SOCKET_ERROR)
	{
		spdlog::error("send failed. Error: {}", GetLastError());
		closesocket_(m_Sockfd);
		WSACleanup_();
		exit(1);
	}
}

void Socket::CloseSocket()
{
	closesocket_(m_Sockfd);
}

void Socket::Shutdown()
{
	int iResult;

	iResult = shutdown(m_Sockfd, 1);
	if (iResult == SOCKET_ERROR)
	{
		spdlog::error("shutdown failed. Error: {}", GetLastError());
		closesocket_(m_Sockfd);
		WSACleanup_();
		exit(1);
	}

	// cleanup
	closesocket_(m_Sockfd);
	WSACleanup_();
}
