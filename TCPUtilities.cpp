#include "./TCPUtilities.h"

namespace marusalib {
namespace tcp {
namespace utilities {

void TCPHost::host_init(uint32_t ip, uint16_t port)
{
	set_data2addr(&(this->addr), ip, port);

	this->socket = create_socket();
}

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


} // namespace utilities
} // namespace tcp
} // namespace marusalib
