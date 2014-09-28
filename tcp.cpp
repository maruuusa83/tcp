#include "./tcp.h"

namespace marusalib{
namespace tcp {

/**
 * Constructor that set receive port no to a given value.
 * \param port_no is a number of message receive port.
 */
TCPServer::TCPServer(int port_no)
{
	/* Create Socket */
	if ((this->listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
#ifdef ___TCP_DEBUG___
		fprintf(stderr, "TCPServer::TCPServer - ERROR, socket() didn't create socket.\n");
#endif /* ___TCP_DEBUG___ */
		return;
	}

	/* If addr isn't zero, perhaps error will occur at bind(). */
	memset(&server_addr, 0, sizeof(server_addr));

	/* Set address and port to socket. */
	this->server_addr.sin_family		= PF_INET;
	this->server_addr.sin_addr.s_addr	= INADDR_ANY;
	this->server_addr.sin_port		= htons(port_no);
	if (bind(this->listen_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0){
#ifdef ___TCP_DEBUG___
		fprintf(stderr, "TCPServer::TCPServer - ERROR, bind() didn't bind addr to socket.\n");
#endif /* ___TCP_DEBUG___ */
		return;
	}

	on_reply_resv_listener = new OnReplyReceiveListener();
}

/**
 * Destructor
 */
TCPServer::~TCPServer()
{
	delete on_reply_resv_listener;
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
	fprintf(stdout, "TCPServer::start_listening - Start Listening Port : %d...\n", this->server_addr.sin_port);
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
		ResvContext *context = new ResvContext;
		context->sock = conn_sock;
		context->listener = on_reply_resv_listener;

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
	close(this->listen_sock);
}

void *TCPServer::recv_msg(void *resv_context)
{
	ResvContext *context = (ResvContext *)resv_context;
	char buf[MAX_MESSAGE_SIZE + 1];

	int conn_sock = context->sock;
	OnReplyReceiveListener *listener = context->listener;

	do {
		int rsize = recv(conn_sock, buf, MAX_MESSAGE_SIZE, 0);

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
			listener->onRecv(conn_sock, buf);
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

void TCPServer::set_on_reply_recv_listener(OnReplyReceiveListener *listener)
{
	delete (this->on_reply_resv_listener);

	this->on_reply_resv_listener = listener;
}


void TCPServer::OnReplyReceiveListener::onRecv(int sock_id, char *msg)
{
	/* nothing to do */
}
TCPServer::OnReplyReceiveListener::~OnReplyReceiveListener()
{
	/* nothing to do */
}

}
}
