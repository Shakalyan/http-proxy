#include "socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "exception.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

Proxy::Socket::Socket(std::string address, int port, int domain, int type) 
        : socketFD(-1), address(std::move(address)), port(port), domain(domain), type(type) {
}

Proxy::Socket::~Socket() {
    if (socketFD != -1) {
        close(socketFD);
    }
}

Proxy::Socket::Socket(Socket const& socket)
        : address(socket.address), port(socket.port), domain(socket.domain), type(socket.type) {
    if (socket.socketFD != -1) {
        this->socketFD = dup(socket.socketFD);
    }
}

Proxy::Socket::Socket(Socket&& socket)
        : socketFD(socket.socketFD), address(std::move(socket.address)), port(socket.port), domain(socket.domain), type(socket.type) {
    socket.socketFD = -1;
}

Proxy::Socket& Proxy::Socket::operator= (Socket const& socket) {
    address = socket.address;
    port = socket.port;
    domain = socket.domain;
    type = socket.domain;

    if (socketFD != -1) {
        close(socketFD);
    }

    if (socket.socketFD != -1) {
        socketFD = dup(socket.socketFD);
    } else {
        socketFD = -1;
    }

    return *this;
}

Proxy::Socket& Proxy::Socket::operator= (Socket&& socket) {
    address = std::move(socket.address);
    port = socket.port;
    domain = socket.domain;
    type = socket.type;

    if (socketFD != -1) {
        close(socketFD);
    }

    socketFD = socket.socketFD;
    socket.socketFD = -1;

    return *this;
}

void Proxy::Socket::listen(int maxConnections, bool reuseAddr) {

    if (this->socketFD != -1) {
        throw ProxyException(EXC_SOCK_IN_USE, "Socket is already in use");
    }

    int socketFD = socket(domain, type, 0);
    if (socketFD == -1) {
        throw ProxyException(EXC_CREATE_SOCKET, strerror(errno));
    }

    int ra = reuseAddr;
    if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &ra, sizeof(ra)) == -1) {
        throw ProxyException(EXC_SETSOCKOPT, strerror(errno));
    }

    sockaddr_in sockAddr;
    sockAddr.sin_family = domain;
    sockAddr.sin_port = port;
    sockAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socketFD, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) == -1) {
        throw ProxyException(EXC_BIND, strerror(errno));
    }

    if (::listen(socketFD, maxConnections) == -1) {
        throw ProxyException(EXC_LISTEN, strerror(errno));
    }

    this->socketFD = socketFD;
}

void Proxy::Socket::connect() {
    if (this->socketFD != -1) {
        throw ProxyException(EXC_SOCK_IN_USE, "Socket is already in use");
    }

    int socketFD = socket(domain, type, 0);
    if (socketFD == -1) {
        throw ProxyException(EXC_CREATE_SOCKET, strerror(errno));
    }

    sockaddr_in sockAddr;
    sockAddr.sin_family = domain;
    sockAddr.sin_port = port;
    if (inet_pton(domain, address.c_str(), &sockAddr.sin_addr.s_addr) == -1) {
        throw ProxyException(EXC_ADDR_CNVRSN, strerror(errno));
    }

    if (::connect(socketFD, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) == -1) {
        throw ProxyException(EXC_CONNECT, strerror(errno));
    }

    this->socketFD = socketFD;
}