#ifndef ___TCP_UTILITIES_H___
#define ___TCP_UTILITIES_H___

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include "./TCPSettings.h"

#ifdef ___TCP_DEBUG___
#include <stdio.h>
#endif /* ___TCP_DEBUG___ */

namespace marusalib {
namespace tcp {
namespace utilities {

/*** prototype ***/
struct _recv_context;
class OnReplyRecvListener;
class TCPHost;

/*** Declaration of Functions ***/
void set_data2addr(struct sockaddr_in *addr, uint32_t ip, uint16_t port);
int create_socket(void);
int create_thread(TCPHost *host, int socket);
void *recv_msg(void *recv_context);
int send_msg(int socket, MESSAGE *msg);

/*** Definition of Structures/Classes ***/
typedef struct _recv_context {
	TCPHost *host;
	int conn_sock;
} RecvContext;

class OnReplyRecvListener {
public:
	virtual void onRecv(RecvContext *context, MESSAGE *msg);
	virtual ~OnReplyRecvListener();
};

class TCPHost {
protected:
	struct sockaddr_in addr;
	int socket;

	OnReplyRecvListener *recv_listener;

public:
	TCPHost(uint32_t ip, uint16_t port);
	virtual ~TCPHost();

	int get_socket(void);
	void set_on_reply_recv_listener(OnReplyRecvListener *listener);
	OnReplyRecvListener *get_on_reply_recv_listener(void);
};

/*** Definition of Const Values ***/
const int MAX_MSG_SIZE = 10000;


} // namespace utilities
} // namespace tcp
} // namespace marusalib

#endif /* ___TCP_UTILITIES_H___ */

