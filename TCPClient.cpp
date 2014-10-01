#include "./tcp.h"

namespace marusalib {
namespace tcp {

TCPClient::TCPClient(uint32_t ip, uint16_t port)
{
	this->port = port;
	this->ip = ip;
	this->socket = utility::create_socket(ip, port);
}

TCPClient::connect()
{
	struct sockadr_in addr;

	

	if (connect(this->socket, (struct sockaddr *)&dstAddr, sizeof(dstAddr)) < 0){

	}
}

}
}
