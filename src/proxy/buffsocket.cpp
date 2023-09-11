#include "buffsocket.h"
#include <utility>
#include <iostream>

Proxy::BuffSocket::BuffSocket(Socket socket, unsigned int MTU, unsigned int tuCount) 
        : socket(std::move(socket)),
          linkedSocket(nullptr),
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
          linkedSocket(buffSocket.linkedSocket),
          MTU(buffSocket.MTU),
          mtub(std::move(buffSocket.mtub)),
          rbuffer(std::move(buffSocket.rbuffer)),
          wbuffer(std::move(buffSocket.wbuffer)),
          wbufferSize(buffSocket.wbufferSize),
          wbufferPtr(buffSocket.wbufferPtr),
          wbufferEmpty(buffSocket.wbufferEmpty),
          rbufferEmpty(buffSocket.rbufferEmpty) {
    buffSocket.linkedSocket = nullptr;
}

Proxy::BuffSocket& Proxy::BuffSocket::operator= (BuffSocket&& buffSocket) {
    socket = std::move(buffSocket.socket);
    linkedSocket = buffSocket.linkedSocket;
    buffSocket.linkedSocket = nullptr;
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

std::unique_ptr<char[]> Proxy::BuffSocket::getReadBuffer(int& size) {
    size = rbuffer.getBufferSize();
    std::unique_ptr<char[]> cstr(new char[size+1]);
    rbuffer.resetPointer();
    rbuffer.read(cstr.get(), size);
    rbuffer.resetPointer();
    rbufferEmpty = true;
    cstr.get()[size] = '\0';
    return cstr;
}

bool Proxy::BuffSocket::wbufferIsEmpty() const {
    return wbufferEmpty;
}

bool Proxy::BuffSocket::rbufferIsEmpty() const {
    return rbufferEmpty;
}

Proxy::BuffSocket* Proxy::BuffSocket::getLinkedSocket() const {
    return linkedSocket;
}

void Proxy::BuffSocket::setLinkedSocket(BuffSocket *socket) {
    linkedSocket = socket;
}