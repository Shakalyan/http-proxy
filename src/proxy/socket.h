#ifndef SOCKET_GUARD
#define SOCKET_GUARD

#include <string>
#include <sys/socket.h>
#include <memory>

namespace Proxy {

    class Socket {
    private:
        int socketFD;
        std::string address;
        int port;
        int domain;
        int type;

    public:

        explicit Socket(std::string address, int port, int domain = AF_INET, int type = SOCK_STREAM) noexcept;
        virtual ~Socket();

        Socket(Socket const& socket) noexcept;
        Socket(Socket&& socket) noexcept;
        Socket& operator= (Socket const& socket);
        Socket& operator= (Socket&& socket);

        virtual void listen(int maxConnections = 16, bool reuseAddr = true);
        virtual void connect();
        virtual Socket accept();

        virtual int read(char *dst, size_t n);
        virtual int write(char *data, size_t n);

        int getSocketFD() const;

    };

}

#endif