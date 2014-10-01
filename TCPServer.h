#ifndef ___TCP_SERVER_H___
#define ___TCP_SERVER_H___

#ifdef ___TCP_DEBUG___
#include <stdio.h>
#endif /* ___TCP_DEBUG___ */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include "./TCPSettings.h"
#include "./TCPUtilities.h"

namespace marusalib {
namespace tcp {

using utilities::TCPHost;
using utilities::OnReplyRecvListener;
using utilities::RecvContext;

class TCPServer : public TCPHost
{
	typedef TCPHost Base;

private:
	OnReplyRecvListener *recv_listener;

	static void *recv_msg(void *recv_context);

public:
	TCPServer(uint16_t port_no);
	~TCPServer();

	void start_listening(void);

	void set_on_reply_recv_listener(OnReplyRecvListener *listener);
	OnReplyRecvListener *get_on_reply_recv_listener(void);
};
 
} // namespace tcp
} // namespace marusalib

#endif /* ___TCP_SERVER_H___ */

