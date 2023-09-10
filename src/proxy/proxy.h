#ifndef PROXY_GUARD
#define PROXY_GUARD

#include "../logger/logger.h"

namespace Proxy {

    class ProxyServer {
    private:
        int port;
        Logger::ILogger &logger;

    public:

        explicit ProxyServer(int port, Logger::ILogger &logger);
        virtual ~ProxyServer();

        void start();
    
    private:

        void logError(const std::string &comment);

    };

}

#endif