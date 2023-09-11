#include "proxy.h"

#include <errno.h>
#include <string.h>
#include "exception.h"
#include "socket.h"
#include "sockmanager.h"

void Proxy::ProxyServer::logError(const std::string &comment) {
    this->logger.log(Logger::ERROR_LEVEL, comment + ": " + strerror(errno));
}

Proxy::ProxyServer::ProxyServer(int port, Logger::ILogger &logger) : port(port), logger(logger) {

}

Proxy::ProxyServer::~ProxyServer() {

}

void Proxy::ProxyServer::start() {

    logger.log(Logger::INFO_LEVEL, "Starting proxy server...");

    try {
        
        Socket serverSocket("localhost", port);
        serverSocket.listen();
        logger.log(Logger::INFO_LEVEL, "Start listening to connections...");
        
        SocketManager socketManager(logger);
        std::vector<BuffSocket> sockets;
        while (true) {
            socketManager.checkSockets(serverSocket, sockets);
        }

    } catch (ProxyException& e) {
        logger.log(Logger::ERROR_LEVEL, e.moveMessage());
    }

}

