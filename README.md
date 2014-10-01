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
using marusalib::tcp::MESSAGE;

class Listener : public TCPServer::OnReplyReceiveListener
{
public:
    void onResv(RecvContext *context, MESSAGE *msg)
    {
        /* If received new message, this function will called. */
        TCPHost *host = context->host;
        host->send_msg(msg);
    }
}

int main(void){
    TCPServer *sv = new TCPServer(1234);

    Listener *listener = new Listener();
    sv->set_on_reply_recv_listener(listener);

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
    struct hostent *hp = gethostbyname(ip_str);
    
    TCPClient *client = new TCPClient(((struct sockaddr_in *)hp->h_addr).s_addr, 1234);

    client->est_conn();
    client->send_msg("Hello, World!");
}
```

