#include "./TCPClient.h"

namespace marusalib {
namespace tcp {

/**
 * Constructor that set receive port no to a given value.
 * \param part_no is a number of message receive port.
 */
TCPClient::TCPClient(uint32_t ip, uint16_t port_no) : TCPHost(ip, port_no)
{
	/* nothing to do */
}

/**
 * Destructor
 */
TCPClient::~TCPClient()
{
	/* nothing to do */
}

int TCPClient::est_conn(void)
{
	socklen_t len = sizeof(struct sockaddr_in);
	if (connect((this->socket), (struct sockaddr *)&(this->addr), len) < 0){
#ifdef ___TCP_DEBUG___
		fprintf(stderr, "TCPClient::est_conn - ERROR, connect() didn't connect to addr : %d.\n", this->addr.sin_addr.s_addr);
#endif /* ___TCP_DEBUG___ */
		return (-1);
	}

	/* Create thread */
	utilities::create_thread(this, this->socket);

	return (0);
}

int TCPClient::send_msg(MESSAGE *msg)
{
	utilities::send_msg(this->socket, msg);

	return (0);
}


} // namespace tcp
} // namespace marusalib
