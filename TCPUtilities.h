#ifndef ___TCP_UTILITIES_H___
#define ___TCP_UTILITIES_H___


#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "./TCPSettings.h"

#ifdef ___TCP_DEBUG___
#include <stdio.h>
#endif /* ___TCP_DEBUG___ */

namespace marusalib {
namespace tcp {
namespace utilities {

/*** Declaration of Functions ***/
void set_data2addr(struct sockaddr_in *addr, uint32_t ip, uint16_t port);
int create_socket(void);

/*** Definition of Structures/Classes ***/
class TCPHost {
protected:
	struct sockaddr_in addr;
	int socket;

public:
	void host_init(uint32_t ip, uint16_t port);
	virtual ~TCPHost();

	int get_socket(void);
};

typedef struct _recv_context {
	TCPHost *host;
	int conn_sock;
} RecvContext;

class OnReplyRecvListener {
public:
	virtual void onRecv(RecvContext *context, MESSAGE *msg);
	virtual ~OnReplyRecvListener();
};

/*** Definition of Const Values ***/
const int MAX_MSG_SIZE = 10000;

} // namespace utilities
} // namespace tcp
} // namespace marusalib

#endif /* ___TCP_UTILITIES_H___ */

