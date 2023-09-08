#ifndef SOCKET_GUARD
#define SOCKET_GUARD

#include <string>
#include <sys/socket.h>

namespace Proxy {

    class Socket {
    private:
        int socketFD;
        std::string address;
        int port;
        int domain;
        int type;

    public:

        explicit Socket(std::string address, int port, int domain = AF_INET, int type = SOCK_STREAM);
        virtual ~Socket();

        Socket(Socket const& socket);
        Socket(Socket&& socket);
        Socket& operator= (Socket const& socket);
        Socket& operator= (Socket&& socket);

        virtual void listen(int maxConnections = 16, bool reuseAddr = true);
        virtual void connect();

    };

}

#endif