#include "httpheader.h"

HTTP::HttpHeader::HttpHeader(std::string const& key, std::string const& value) 
        : key(key), value(value) {
}

HTTP::HttpHeader::HttpHeader(std::string&& key, std::string&& value) 
        : key(std::move(key)), value(std::move(value)) {
}

HTTP::HttpHeader::HttpHeader(HttpHeader const& httpHeader) 
        : key(httpHeader.key), value(httpHeader.value) {
}

HTTP::HttpHeader::HttpHeader(HttpHeader&& httpHeader) 
        : key(std::move(httpHeader.key)), value(std::move(httpHeader.value)) {
}

HTTP::HttpHeader::~HttpHeader() {

}

HTTP::HttpHeader& HTTP::HttpHeader::operator=(HttpHeader const& httpHeader) {
    key = httpHeader.key;
    value = httpHeader.value;
    return *this;
}

HTTP::HttpHeader& HTTP::HttpHeader::operator=(HttpHeader&& httpHeader) {
    key = std::move(httpHeader.key);
    value = std::move(httpHeader.value);
    return *this;
}

std::string HTTP::HttpHeader::getKey() const {
    return key;
}

std::string HTTP::HttpHeader::getValue() const {
    return value;
}

