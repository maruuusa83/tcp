tcp
===================

This library is wrapper of "sys/socket.h".  
This library has two classes : TCPServer and TCPClient.

# Example Codes
## TCPServer
``` c++
#include "./TCPServer.h"

using marusalib::tcp::TCPServer;
using marusalib::tcp::utilities::RecvContext;
using marusalib::tcp::utilities::OnReplyReceiveListener;
using marusalib::tcp::utilities::send_msg;
using marusalib::tcp::MESSAGE;

class Listener : public TCPServer::OnReplyReceiveListener
{
public:
    void onResv(RecvContext *context, MESSAGE *msg)
    {
        /* If received new message, this function will called. */
        printf("recv : %s\n", msg);
        send_msg(context->conn_sock, msg);
    }
}

int main(void){
    TCPServer *sv = new TCPServer(1234);

    Listener *listener = new Listener();
    sv->set_on_reply_recv_listener((OnReplyReceiveListener *)listener);

    sv->start_listening();
}
```

## TCPClient
``` c++
#include "./TCPClient.h"

using marusalib::tcp::TCPClient;
using marusalib::tcp::utilities::RecvContext;
using marusalib::tcp::MESSAGE;

class Listener : public TCPServer::OnReplyReceiveListener
{
public:
    void onResv(RecvContext *context, MESSAGE *msg)
    {
        /* If received new message, this function will called. */
        printf("%s\n", msg);
    }
}

int main(void){
    char ip_str[] = "127.0.0.1";
    int port_no = 1234;

    TCPClient *client = new TCPClient(inet_addr(ip_str), port_no);

    Listener *listener = new Listener();
    client->set_on_reply_recv_listener((OnReplyRecvListener *)listener);

    client->est_conn();
    client->send_msg("Hello, World!");

    while (1);
}
```

