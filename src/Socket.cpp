#include "Socket.h"

// some specific code for winsock and BSD sockets
static void closesocket_(SOCKET sockToClose)
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

static void LogError(const char* errorMsg)
{
	std::cout << "[ERROR] " << errorMsg << " WSAError: " << WSAGetLastError() << std::endl;
}

static void LogError(const char* errorMsg, int iResult)
{
	std::cout << "[ERROR] " << errorMsg << " " << iResult << std::endl;
}

static void LogInfo(const char* infoMsg)
{
	std::cout << "[INFO] " << infoMsg << std::endl;
}

// inits and binds socket
Socket::Socket(const char* iNode, const char* iPort)
{
	int iResult;
	#ifdef WIN32
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
			LogError("Failed WSAStartup with code", iResult);
			WSACleanup_();
			exit(1);
		}
	#endif

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, iPort, &hints, &result);
	if (iResult != 0)
	{
		LogError("getaddrinfo failed");
		freeaddrinfo(result);
		WSACleanup_();
		exit(1);
	}

	// create listening socket
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET)
	{
		LogError("socket failed");
		freeaddrinfo(result);
		WSACleanup_();
		exit(1);
	}

	// bind socket
	iResult = bind(listenSocket, result->ai_addr, (int) result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		LogError("bind failed");
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
	int iResult;

	iResult = listen(listenSocket, backlog);
	if (iResult != 0)
	{
		LogError("listen failed");
		closesocket_(listenSocket);
		WSACleanup_();
		exit(1);
	}
}

void Socket::Accept()
{
	clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET)
	{
		LogError("accept failed");
		closesocket_(listenSocket);
		WSACleanup_();
		exit(1);
	}

	closesocket_(listenSocket); // close listen socket because conn established
}

std::string Socket::Recv(int bytesToRecv)
{
	int iResult; // => number of bytes recv'd
	std::string recvData;
	recvData.resize(bytesToRecv);

	iResult = recv(clientSocket, recvData.data(), bytesToRecv, 0);
	if (iResult > 0)
	{
		LogInfo((std::string("Received bytes: ") + std::to_string(iResult)).c_str());
	}
	else if (iResult == 0)
		LogInfo("Closing connection");
	else
	{
		// failed
		LogError("recv failed");
		closesocket_(clientSocket);
		WSACleanup_();
		exit(1);
	}

	return recvData;
}

void Socket::Send(std::string& sendString)
{
	int iResult;

	iResult = send(clientSocket, sendString.c_str(), sendString.length(), 0);
	LogInfo((std::string("Bytes sent: ") + std::to_string(iResult)).c_str());
	if (iResult == SOCKET_ERROR)
	{
		LogError("send failed");
		closesocket_(clientSocket);
		WSACleanup_();
		exit(1);
	}
}

void Socket::Shutdown()
{
	int iResult;

	iResult = shutdown(clientSocket, 1);
	if (iResult == SOCKET_ERROR)
	{
		LogError("shutdown failed");
		closesocket_(clientSocket);
		WSACleanup_();
		exit(1);
	}

	// cleanup
	closesocket_(clientSocket);
	WSACleanup_();
}
