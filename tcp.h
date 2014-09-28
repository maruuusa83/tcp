#ifndef ___TCP_H___
#define ___TCP_H___

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

namespace marusalib{
namespace tcp {

#define ___TCP_DEBUG___

class TCPServer
{
private:
	struct _resv_context;
	typedef struct _resv_context ResvContext;

public:
	class OnReplyReceiveListener;


private:
	struct sockaddr_in server_addr;
	int listen_sock;
	OnReplyReceiveListener *on_reply_resv_listener;

	static void *recv_msg(void *resv_context);

	struct _resv_context{ /* This struct will rename to ResvContext by typedef */
		int sock;
		OnReplyReceiveListener *listener;
	};

public:
	TCPServer(int port_no);
	~TCPServer();
	void start_listening(void);
	void set_on_reply_recv_listener(OnReplyReceiveListener *listener);

	class OnReplyReceiveListener{
	public:
		virtual void onRecv(int sock_id, char *msg);
		virtual ~OnReplyReceiveListener();
	};

	static const int MAX_MESSAGE_SIZE = 10000;
};

}
}

#endif /* ___TCP_H___ */
