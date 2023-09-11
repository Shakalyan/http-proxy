#include "sockmanager.h"
#include "exception.h"
#include "../http/httprequest.h"

#include <string.h>
#include <unistd.h>
#include <iostream>

Proxy::SocketManager::SocketManager(Logger::ILogger& logger) : logger(logger) {
}

Proxy::SocketManager::~SocketManager() {

}

int Proxy::SocketManager::prepareFDs(Socket& serverSocket, std::vector<BuffSocket>& sockets) {
    int maxFD = serverSocket.getSocketFD();

    FD_ZERO(&readFDs);
    FD_ZERO(&writeFDs);    
    FD_SET(serverSocket.getSocketFD(), &readFDs);
    for (BuffSocket const& socket : sockets) {
        int socketFD = socket.getSocketFD();
        maxFD = (maxFD < socketFD) ? socketFD : maxFD;
        if (socket.wbufferIsEmpty()) {
            FD_SET(socketFD, &readFDs);
        }
        if (socket.rbufferIsEmpty() && !socket.wbufferIsEmpty()) {
            FD_SET(socketFD, &writeFDs);
        }
    }

    return maxFD;
}

void Proxy::SocketManager::handleRequest(std::string const& requestData, int socketIndex, std::vector<BuffSocket>& sockets) {
    if (sockets[socketIndex].getLinkedSocket() != nullptr) {
        std::cout << "LINKED SOCKET IS NOT NULLPTR\n";
        return;
    }
    HTTP::HttpRequest request = HTTP::parseHttpRequest(requestData);
    Socket newSocket(request.getDstHost(), request.getDstPort());
    newSocket.connect();
    sockets.push_back(BuffSocket(std::move(newSocket)));
    sockets[socketIndex].setLinkedSocket(&sockets.back());
    sockets.back().setLinkedSocket(&sockets[socketIndex]);
}

void Proxy::SocketManager::checkSockets(Socket& serverSocket, std::vector<BuffSocket>& sockets) {
    int maxFD = prepareFDs(serverSocket, sockets);

    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100;
    select(maxFD+1, &readFDs, &writeFDs, NULL, &tv);

    int socketsCnt = sockets.size();
    for (int i = 0; i < socketsCnt; ++i) {
        if (FD_ISSET(sockets[i].getSocketFD(), &readFDs)) {
            if (!sockets[i].read()) {
                sockets[i] = std::move(sockets[socketsCnt-1]);
                --socketsCnt;
                --i;
                sockets.pop_back();
                logger.log(Logger::INFO_LEVEL, "Disconnected");
            }

        } else if (!sockets[i].rbufferIsEmpty()) {
            int readBufferSize = 0;
            std::unique_ptr<char[]> requestRaw = sockets[i].getReadBuffer(readBufferSize);
            std::string requestStr(requestRaw.get());
            logger.log(Logger::INFO_LEVEL, "Request:\n" + requestStr);
            handleRequest(requestStr, i, sockets);
            sockets[i].getLinkedSocket()->setWriteBuffer(std::move(requestRaw), readBufferSize);
            sockets[i].getLinkedSocket()->write();            

        } else if (FD_ISSET(sockets[i].getSocketFD(), &writeFDs)) {
            sockets[i].write();
        }
    }

    if (FD_ISSET(serverSocket.getSocketFD(), &readFDs)) {
        sockets.push_back(BuffSocket(serverSocket.accept()));
        logger.log(Logger::INFO_LEVEL, "Accepted new connection");
    }

}