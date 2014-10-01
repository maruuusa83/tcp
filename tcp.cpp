#include "./tcp.h"

namespace marusalib {
namespace tcp {

namespace utility {
int create_socket()
{
	int sock;

	/* Create Socket */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
#ifdef ___TCP_DEBUG___
		fprintf(stderr, "TCPServer::TCPServer - ERROR, socket() didn't create socket.\n");
#endif /* ___TCP_DEBUG___ */
		return (-1);
	}

	return (sock);
}

int bind_addr(uint32_t ip, uint16_t port)
{
	struct sockaddr_in addr;
	/* If addr isn't zero, perhaps error will occur at bind(). */
	memset(&addr, 0, sizeof(addr));

	/* Set address and port to socket. */
	addr.sin_family			= PF_INET;
	addr.sin_addr.s_addr	= ip;
	addr.sin_port			= htons(port);
	if (bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0){
#ifdef ___TCP_DEBUG___
		fprintf(stderr, "TCPServer::TCPServer - ERROR, bind() didn't bind addr to socket.\n");
#endif /* ___TCP_DEBUG___ */
		return (-1);
	}

	return (sock);
}
}

/**
 * Constructor that set receive port no to a given value.
 * \param port_no is a number of message receive port.
 */
TCPServer::TCPServer(int port_no)
{
	this->port = port_no;
	this->listen_sock = utility::create_socket(INADDR_ANY, port_no);
	on_reply_recv_listener = new OnReplyReceiveListener();
}

/**
 * Destructor
 */
TCPServer::~TCPServer()
{
	delete on_reply_recv_listener;
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
	/* Start port listening */
	if (listen(this->listen_sock, SOMAXCONN) < 0){
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

		if ((conn_sock = accept(this->listen_sock, (struct sockaddr *)&client_addr, &len)) < 0){
#ifdef ___TCP_DEBUG___
			fprintf(stderr, "TCPServer::TCPServer - ERROR, accept() didn't accept client.\n");
#endif /* ___TCP_DEBUG___ */
			return;
		}

		/* Create thread */
		pthread_t worker;
		if (pthread_create(&worker, NULL, this->recv_msg, (void *)this) != 0){
#ifdef ___TCP_DEBUG___
			fprintf(stderr, "TCPServer::TCPServer - ERROR, accept() didn't accept client.\n");
#endif /* ___TCP_DEBUG___ */
			return;
		}
		pthread_detach(worker);
	}

	/* Close listening socket */
	close(this->listen_sock);
}

void *TCPServer::recv_msg(void *recv_context)
{
	char buf[utility::MAX_MESSAGE_SIZE + 1];

	TCPServer *context = (TCPServer *)recv_context;
	int conn_sock = context->listen_sock;
	OnReplyReceiveListener *listener = context->get_on_reply_recv_listener();

	do {
		int rsize = recv(conn_sock, buf, utility::MAX_MESSAGE_SIZE, 0);

		if (rsize == 0){
#ifdef ___TCP_DEBUG___
			fprintf(stdout, "TCPServer::recv_msg - Disconnected client.\n");
#endif /* ___TCP_DEBUG___ */
			break;
		}
		else if (rsize == -1){
#ifdef ___TCP_DEBUG___
			fprintf(stderr, "TCPServer::recv_msg - ERROR, recv\n");
#endif /* ___TCP_DEBUG___ */
			return (NULL);
		}
		else {
			listener->onRecv(context, buf);
		}
	} while (1);

	if (close(conn_sock) < 0){
#ifdef ___TCP_DEBUG___
		fprintf(stderr, "TCPServer::recv_msg - ERROR, recv\n");
#endif /* ___TCP_DEBUG___ */
		return (NULL);
	}

#ifdef ___TCP_DEBUG___
	fprintf(stdout, "TCPServer::recv_msg - Connection closed.\n");
#endif /* ___TCP_DEBUG___ */

	return (NULL);
}

void TCPServer::set_on_reply_recv_listener(TCPServer::OnReplyReceiveListener *listener)
{
	delete (this->on_reply_recv_listener);

	this->on_reply_recv_listener = listener;
}

TCPServer::OnReplyReceiveListener *TCPServer::get_on_reply_recv_listener(void)
{
	return (this->on_reply_recv_listener);
}


void TCPServer::OnReplyReceiveListener::onRecv(TCPServer *context, char *msg)
{
	/* nothing to do */
}
TCPServer::OnReplyReceiveListener::~OnReplyReceiveListener()
{
	/* nothing to do */
}

}
}
