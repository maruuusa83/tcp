#include "./TCPServer.h"

namespace marusalib {
namespace tcp {


/**
 * Constructor that set receive port no to a given value.
 * \param part_no is a number of message receive port.
 */
TCPServer::TCPServer(uint16_t port_no) : TCPHost(INADDR_ANY, port_no)
{
	/* nothing to do */
}

/**
 * Destructor
 */
TCPServer::~TCPServer()
{
	/* nothing to do */
}

/**
 * Start listening and create connection thread.
 * <pre>
 * After you call this method, TCPServer will start listening socket.
 * This method create new thread if new client is attempt connecting.
 * </pre>
 */
void TCPServer::start_listening(void)
{
	if (bind(this->socket, (struct sockaddr *)&(this->addr), sizeof(struct sockaddr_in)) < 0){
#ifdef ___TCP_DEBUG___
		fprintf(stderr, "TCPServer::start_listening - ERROR, bind() didn't bind addr to socket.\n");
#endif /* ___TCP_DEBUG___ */
		return;
	}

	if (listen(this->socket, SOMAXCONN) < 0){
#ifdef ___TCP_DEBUG___
		fprintf(stderr, "TCPServer::start_listening - ERROR, listen() didn't start listening.\n");
#endif /* ___TCP_DEBUG___ */
		return;
	}
#ifdef ___TCP_DEBUG___
	fprintf(stdout, "TCPServer::start_listening - Start Listening Port : %d...\n", ntohs(this->addr.sin_port));
#endif /* ___TCP_DEBUG___ */

	while (1){
		/* Start accept connect request */
		struct sockaddr_in client_addr;
		int conn_sock;
		socklen_t len = sizeof(struct sockaddr_in);

		if ((conn_sock = accept(this->socket, (struct sockaddr *)&client_addr, &len)) < 0){
#ifdef ___TCP_DEBUG___
			fprintf(stderr, "TCPServer::start_listening - ERROR, accept() didn't accept client.\n");
#endif /* ___TCP_DEBUG___ */
			return;
		}

		/* Create thread */
		utilities::create_thread(this, conn_sock);
	}

	/* Close listening socket */
	close(this->socket);
}


} // namespace tcp
} // namespace marusalib
