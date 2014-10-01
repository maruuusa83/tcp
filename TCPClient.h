#ifndef ___TCP_CLIENT_H___
#define ___TCP_CLIENT_H___

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "./TCPSettings.h"
#include "./TCPUtilities.h"

#ifdef ___TCP_DEBUG___
#include <stdio.h>
#endif /* ___TCP_DEBUG___ */

namespace marusalib {
namespace tcp {

using utilities::TCPHost;
using utilities::OnReplyRecvListener;
using utilities::RecvContext;

class TCPClient : public TCPHost
{
	typedef TCPHost Base;

public:
	TCPClient(uint32_t ip, uint16_t port_no);
	~TCPClient();

	int est_conn(void);
};
 
} // namespace tcp
} // namespace marusalib

#endif /* ___TCP_CLIENT_H___ */


