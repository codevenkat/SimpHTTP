#include "Socket.h"

int main()
{
    Socket socket;
    socket.Bind(NULL, "4000");
    socket.Listen(1);
    Socket conn = socket.Accept();
    std::string dat = conn.Recv(512);
    std::string sendStr = "You said " + dat;

    conn.Send(sendStr);

    conn.Shutdown();

    return 0;
}
