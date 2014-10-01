#include "./TCPServer.h"

namespace marusalib {
namespace tcp {

void *TCPServer::recv_msg(void *recv_context)
{
	char buf[utilities::MAX_MSG_SIZE + 1];

	RecvContext *context = (RecvContext *)recv_context;
	int socket = context->host->get_socket();
	OnReplyRecvListener *listener = ((TCPServer *)(context->host))->get_on_reply_recv_listener();
	
	do {
		int rsize = recv(socket, buf, utilities::MAX_MSG_SIZE, 0);

		if (rsize == 0){
#ifdef ___TCP_DEBUG___
			fprintf(stdout, "TCPServer::recv_msg - Disconnected client.\n");
#endif /* ___TCP_DEBUG___ */
			break;
		}
		else if (rsize < 0){
#ifdef ___TCP_DEBUG___
			fprintf(stderr, "TCPServer::recv_msg - ERROR, recv\n");
#endif /* ___TCP_DEBUG___ */
			return (NULL);
		}
		else {
			listener->onRecv(context, buf);
		}
	} while (1);

#ifdef ___TCP_DEBUG___
	fprintf(stdout, "TCPServer::recv_msg - Connection closed.\n");
#endif /* ___TCP_DEBUG___ */

	return (NULL);
}

TCPServer::TCPServer(uint16_t port_no)
{
	host_init(INADDR_ANY, port_no);
	recv_listener = new OnReplyRecvListener();
}

TCPServer::~TCPServer()
{
	delete (recv_listener);
}

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
	fprintf(stdout, "TCPServer::start_listening - Start Listening Port : %d...\n", this->port);
#endif /* ___TCP_DEBUG___ */

	while (1){
		/* Start accept connect request */
		struct sockaddr_in client_addr;
		int conn_sock;
		socklen_t len = sizeof(struct sockaddr_in);

		if ((conn_sock = accept(this->socket, (struct sockaddr *)&client_addr, &len)) < 0){
#ifdef ___TCP_DEBUG___
			fprintf(stderr, "TCPServer::TCPServer - ERROR, accept() didn't accept client.\n");
#endif /* ___TCP_DEBUG___ */
			return;
		}

		/* Create thread */
		RecvContext *context = new RecvContext();
		context->host = this;
		context->conn_sock = conn_sock;

		pthread_t worker;
		if (pthread_create(&worker, NULL, this->recv_msg, (void *)context) != 0){
#ifdef ___TCP_DEBUG___
			fprintf(stderr, "TCPServer::TCPServer - ERROR, accept() didn't accept client.\n");
#endif /* ___TCP_DEBUG___ */
			return;
		}
		pthread_detach(worker);
	}

	/* Close listening socket */
	close(this->socket);
}

void TCPServer::set_on_reply_recv_listener(OnReplyRecvListener *listener)
{
	this->recv_listener = listener;
}

} // namespace tcp
} // namespace marusalib
