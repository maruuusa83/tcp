#ifndef ___TCP_H___
#define ___TCP_H___

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#define ___TCP_DEBUG___

namespace marusalib {
namespace tcp {

namespace utility {
int create_socket(uint32_t ip, uint16_t port);
const int MAX_MESSAGE_SIZE = 10000;
}

class TCPServer
{
public:
	class OnReplyReceiveListener;


private:
	uint16_t port;
	int listen_sock;
	OnReplyReceiveListener *on_reply_recv_listener;

	static void *recv_msg(void *resv_context);

public:
	TCPServer(int port_no);
	~TCPServer();
	void start_listening(void);
	void set_on_reply_recv_listener(OnReplyReceiveListener *listener);
	OnReplyReceiveListener *get_on_reply_recv_listener();

	class OnReplyReceiveListener{
	public:
		virtual void onRecv(TCPServer *context, char *msg);
		virtual ~OnReplyReceiveListener();
	};
};

}
}

#endif /* ___TCP_H___ */
