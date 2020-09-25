#include "Socket.h"

int main()
{
    Socket socket(NULL, "4000");
    socket.Listen(1);
    socket.Accept();
    std::string dat = socket.Recv(512);

    std::string sendStr = "You said " + dat;

    socket.Send(sendStr);

    socket.Shutdown();

    return 0;
}
