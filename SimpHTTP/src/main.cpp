#include "Socket.h"

int main()
{
    Socket socket(NULL, "4000");
    socket.Listen(1);
    socket.Accept();
    std::string dat = socket.Recv(512);
    socket.Send("hello");
    socket.Shutdown();

    return 0;
}
