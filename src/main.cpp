#include "Socket.h"
#include <thread>
#include <unordered_map>
#include <string_view>

constexpr int CONN_BACKLOG = 2;

Socket listenSocket;

void HandleConnection(Socket& connSock)
{
    std::string recvData = connSock.Recv(2048);

    connSock.Send(recvData);
    connSock.Shutdown();
}

int main()
{
    listenSocket.Bind(NULL, "4000");
    listenSocket.Listen(CONN_BACKLOG);

    while (true)
    {
        Socket connSock = listenSocket.Accept();

        std::thread connWorker(HandleConnection, connSock);
        connWorker.join();
    }

    listenSocket.CloseSocket();

    return 0;
}
