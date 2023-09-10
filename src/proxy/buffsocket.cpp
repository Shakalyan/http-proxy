#include "buffsocket.h"
#include <utility>

Proxy::BuffSocket::BuffSocket(Socket socket, unsigned int MTU, unsigned int tuCount) 
        : socket(std::move(socket)), 
          MTU(MTU), 
          mtub(new char[MTU]), 
          rbuffer(MTU * tuCount),
          wbufferSize(0),
          wbufferPtr(0),
          wbufferEmpty(true),
          rbufferEmpty(true) {

}

Proxy::BuffSocket::~BuffSocket() {
}

Proxy::BuffSocket::BuffSocket(BuffSocket&& buffSocket) 
        : socket(std::move(buffSocket.socket)),
          MTU(buffSocket.MTU),
          mtub(std::move(buffSocket.mtub)),
          rbuffer(std::move(buffSocket.rbuffer)),
          wbuffer(std::move(buffSocket.wbuffer)),
          wbufferSize(buffSocket.wbufferSize),
          wbufferPtr(buffSocket.wbufferPtr),
          wbufferEmpty(buffSocket.wbufferEmpty),
          rbufferEmpty(buffSocket.rbufferEmpty) {

}

Proxy::BuffSocket& Proxy::BuffSocket::operator= (BuffSocket&& buffSocket) {
    socket = std::move(buffSocket.socket);
    MTU = buffSocket.MTU;
    mtub = std::move(buffSocket.mtub);
    rbuffer = std::move(buffSocket.rbuffer);
    wbuffer = std::move(buffSocket.wbuffer);
    wbufferSize = buffSocket.wbufferSize;
    wbufferPtr = buffSocket.wbufferPtr;
    wbufferEmpty = buffSocket.wbufferEmpty;
    rbufferEmpty = buffSocket.rbufferEmpty;
    return *this;
}

bool Proxy::BuffSocket::read() {
    int cnt = socket.read(mtub.get(), MTU);
    if (cnt <= 0) {
        return false;
    }
    rbuffer.write(mtub.get(), cnt);
    rbufferEmpty = false;
    return true;
}

bool Proxy::BuffSocket::write() {
    int cnt = socket.write(wbuffer.get() + wbufferPtr, wbufferSize);
    if (cnt == -1) {
        return false;
    } 
    if (cnt == 0) {
        wbufferEmpty = true;
        return false; // sure?
    }
    wbufferPtr += cnt;
    if (wbufferPtr == wbufferSize) {
        wbufferEmpty = true;
    }
    return true;
}

int Proxy::BuffSocket::getSocketFD() const {
    return socket.getSocketFD();
}

void Proxy::BuffSocket::setWriteBuffer(std::unique_ptr<char[]> data, int nbytes) {
    wbuffer = std::move(data);
    wbufferPtr = 0;
    wbufferSize = nbytes;
    wbufferEmpty = false;
}

std::string Proxy::BuffSocket::getReadBuffer() {
    int bsize = rbuffer.getBufferSize();
    std::unique_ptr<char[]> cstr(new char[bsize+1]);
    rbuffer.resetPointer();
    rbuffer.read(cstr.get(), bsize);
    rbuffer.resetPointer();
    rbufferEmpty = true;
    cstr.get()[bsize] = '\0';
    return std::string(cstr.get());
}

bool Proxy::BuffSocket::wbufferIsEmpty() const {
    return wbufferEmpty;
}

bool Proxy::BuffSocket::rbufferIsEmpty() const {
    return rbufferEmpty;
}
