#ifndef BUFFSOCKET_GUARD
#define BUFFSOCKET_GUARD

#include "socket.h"
#include "../utils/rwbuffer.h"

namespace Proxy {

    class BuffSocket {
    private:
        Socket socket;
        unsigned int MTU;

        std::unique_ptr<char[]> mtub;
        Utils::RWBuffer rbuffer;

        std::unique_ptr<char[]> wbuffer;
        int wbufferSize;
        int wbufferPtr;

        bool wbufferEmpty;
        bool rbufferEmpty;

    public:

        BuffSocket(BuffSocket const& buffSocket) = delete;
        BuffSocket& operator= (BuffSocket const& BuffSocket) = delete;

        explicit BuffSocket(Socket socket, unsigned int MTU = 1500, unsigned int tuCount = 4);
        BuffSocket(BuffSocket&& buffSocket);
        BuffSocket& operator= (BuffSocket&& BuffSocket);
        virtual ~BuffSocket();

        virtual bool read();
        virtual bool write();
        
        void setWriteBuffer(std::unique_ptr<char[]> data, int nbytes);
        std::string getReadBuffer();
        int getSocketFD() const;

        bool wbufferIsEmpty() const;
        bool rbufferIsEmpty() const;

    };

}

#endif