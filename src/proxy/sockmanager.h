#ifndef SOCKMANAGER_GUARD
#define SOCKMANAGER_GUARD

#include "socket.h"
#include "buffsocket.h"
#include "../logger/logger.h"
#include <vector>
#include <sys/select.h>

namespace Proxy {

    class SocketManager {
    private:
        fd_set readFDs;
        fd_set writeFDs;
        Logger::ILogger& logger;

    public:

        SocketManager(SocketManager const&) = delete;
        SocketManager& operator= (SocketManager const&) = delete;

        SocketManager(Logger::ILogger& logger);
        ~SocketManager();

        void checkSockets(Socket& serverSocket, std::vector<BuffSocket>& sockets);
    
    private:
        int prepareFDs(Socket& serverSocket, std::vector<BuffSocket>& sockets);

    };

}

#endif