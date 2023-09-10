#include "sockmanager.h"
#include "exception.h"

#include <string.h>
#include <unistd.h>

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

void Proxy::SocketManager::checkSockets(Socket& serverSocket, std::vector<BuffSocket>& sockets) {
    int maxFD = prepareFDs(serverSocket, sockets);

    select(maxFD+1, &readFDs, &writeFDs, NULL, NULL);

    int socketsCnt = sockets.size();
    for (int i = 0; i < socketsCnt; ++i) {
        if (FD_ISSET(sockets[i].getSocketFD(), &readFDs)) {
            if (!sockets[i].read()) {
                sockets[i] = std::move(sockets[socketsCnt-1]);
                --socketsCnt;
                --i;
                sockets.pop_back();
                logger.log(Logger::INFO_LEVEL, "Disconnected");
                continue;
            }
        } else if (!sockets[i].rbufferIsEmpty()) {
            //handle
            logger.log(Logger::INFO_LEVEL, "Request:\n" + sockets[i].getReadBuffer());
        }

        if (FD_ISSET(sockets[i].getSocketFD(), &writeFDs)) {
            sockets[i].write();
        }
    }

    if (FD_ISSET(serverSocket.getSocketFD(), &readFDs)) {
        sockets.push_back(BuffSocket(serverSocket.accept()));
        logger.log(Logger::INFO_LEVEL, "Accepted new connection");
    }

}