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
	RecvContext *context = new RecvContext();
	context->host = this;
	context->conn_sock = this->socket;

	pthread_t worker;
	if (pthread_create(&worker, NULL, utilities::recv_msg, (void *)context) != 0){
#ifdef ___TCP_DEBUG___
		fprintf(stderr, "TCPClient::est_conn - ERROR, didn't create new thread.\n");
#endif /* ___TCP_DEBUG___ */
		return (-1);
	}
	pthread_detach(worker);

	return (0);
}


} // namespace tcp
} // namespace marusalib
