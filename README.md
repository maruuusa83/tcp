tcp
===================

This library is wrapper of "sys.socket.h".  
This library has two classes : TCPServer and TCPClient.

# How to use
## TCPServer
``` c++
using marusalib::tcp::TCPServer;

class Listener : public TCPServer::OnReplyReceiveListener
{
public:
    void onResv(int sock_id, char *msg)
    {
        /* If received new message, this function will called. */
    }
}

int main(void){
    TCPServer *sv = new TCPServer(1234);

    Listener *listener = new Listener();
    sv->set_on_reply_recv_listener(listener);

    sv->start_listening();
}
```
