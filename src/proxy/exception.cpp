#include "exception.h"

Proxy::ProxyException::ProxyException(ExceptionType type, std::string message) 
        : type(type), message(std::move(message)) {

}

Proxy::ProxyException::~ProxyException() {

}

Proxy::ExceptionType Proxy::ProxyException::getType() const {
    return this->type;
}

std::string Proxy::ProxyException::getMessage() const {
    return message;
}

std::string Proxy::ProxyException::moveMessage() {
    return std::move(message);
}