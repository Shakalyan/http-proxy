#ifndef EXCEPTION_GUARD
#define EXCEPTION_GUARD

#include <string>

namespace Proxy {

    enum ExceptionType {
        EXC_CREATE_SOCKET,
        EXC_BIND,
        EXC_LISTEN,
        EXC_SETSOCKOPT,
        EXC_ADDR_CNVRSN,
        EXC_SOCK_IN_USE,
        EXC_CONNECT,
        EXC_INVALID_SOCK,
        EXC_ACCEPT
    };

    class ProxyException {
    private:
        ExceptionType type;
        std::string message;

    public:

        explicit ProxyException(ExceptionType type, std::string message);
        virtual ~ProxyException();

        ExceptionType getType() const;
        std::string getMessage() const;
        std::string moveMessage();

    };

}

#endif