#include "./TCPUtilities.h"

namespace marusalib {
namespace tcp {
namespace utilities {


void set_data2addr(struct sockaddr_in *addr, uint32_t ip, uint16_t port)
{
	/* If addr isn't zero, perhaps error will occur at bind(). */
	memset(addr, 0, sizeof(struct sockaddr_in));

	/* Set address and port to socket. */
	addr->sin_family		= PF_INET;
	addr->sin_addr.s_addr	= ip;
	addr->sin_port			= htons(port);
}

int create_socket(void)
{
	int sock;

	/* Create Socket */
#ifdef ___TCP_DEBUG___
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "utilities::create_socket - ERROR, socket() didn't create socket.\n");
	}
#else
	sock = socket(AF_INET, SOCK_STREAM, 0);
#endif /* ___TCP_DEBUG___ */

	return (sock);
}

int create_thread(TCPHost *host, int socket)
{
	/* Create thread */
	RecvContext *context = new RecvContext();
	context->host = host;
	context->conn_sock = socket;

	pthread_t worker;
	if (pthread_create(&worker, NULL, recv_msg, (void *)context) != 0){
#ifdef ___TCP_DEBUG___
		fprintf(stderr, "utilities::create_thread - ERROR, didn't create new thread.\n");
#endif /* ___TCP_DEBUG___ */
		return (-1);
	}
	pthread_detach(worker);

	return (0);
}

void *recv_msg(void *recv_context)
{
	char buf[utilities::MAX_MSG_SIZE + 1];

	RecvContext *context = (RecvContext *)recv_context;
	int socket = context->conn_sock;
	OnReplyRecvListener *listener = context->host->get_on_reply_recv_listener();
	
	do {
		int rsize = recv(socket, buf, utilities::MAX_MSG_SIZE, 0);

		if (rsize == 0){
#ifdef ___TCP_DEBUG___
			fprintf(stdout, "utilities::recv_msg - Disconnected client.\n");
#endif /* ___TCP_DEBUG___ */
			break;
		}
		else if (rsize < 0){
#ifdef ___TCP_DEBUG___
			fprintf(stderr, "utilities::recv_msg - ERROR, recv %d\n", rsize);
#endif /* ___TCP_DEBUG___ */
			return (NULL);
		}
		else {
			listener->onRecv(context, buf);
		}
	} while (1);

#ifdef ___TCP_DEBUG___
	fprintf(stdout, "utilities::recv_msg - Connection closed.\n");
#endif /* ___TCP_DEBUG___ */

	return (NULL);
}

TCPHost::TCPHost(uint32_t ip, uint16_t port)
{
	set_data2addr(&(this->addr), ip, port);
	this->socket = create_socket();

	recv_listener = new OnReplyRecvListener();
}

TCPHost::~TCPHost()
{
	delete (recv_listener);
}

int TCPHost::get_socket(void)
{
	return (this->socket);
}

void TCPHost::set_on_reply_recv_listener(OnReplyRecvListener *listener)
{
	this->recv_listener = listener;
}

OnReplyRecvListener *TCPHost::get_on_reply_recv_listener(void)
{
	return (this->recv_listener);
}

void OnReplyRecvListener::onRecv(RecvContext *context, MESSAGE *msg)
{
	/* nothing to do */
}

OnReplyRecvListener::~OnReplyRecvListener()
{
	/* nothing to do */
}


} // namespace utilities
} // namespace tcp
} // namespace marusalib

