#include "httprequest.h"

HTTP::HttpRequest::HttpRequest(HttpMethod method, 
                               std::string const& dstHost,
                               int dstPort, 
                               std::string const& urn, 
                               std::vector<HttpHeader> const& headers, 
                               std::string const& content) 
                               :
                               method(method),
                               dstHost(dstHost),
                               dstPort(dstPort),
                               urn(urn),
                               headers(headers),
                               content(content) {

}

HTTP::HttpRequest::HttpRequest(HttpMethod method, 
                               std::string&& dstHost,
                               int dstPort, 
                               std::string&& urn, 
                               std::vector<HttpHeader>&& headers, 
                               std::string&& content) 
                               :
                               method(method),
                               dstHost(std::move(dstHost)),
                               dstPort(dstPort),
                               urn(std::move(urn)),
                               headers(std::move(headers)),
                               content(std::move(content)) {

}

HTTP::HttpRequest::HttpRequest(HttpRequest&& request) 
                               :
                               method(request.method),
                               dstHost(std::move(request.dstHost)),
                               dstPort(request.dstPort),
                               urn(std::move(request.urn)),
                               headers(std::move(request.headers)),
                               content(std::move(request.content)) {

}

HTTP::HttpRequest& HTTP::HttpRequest::operator= (HttpRequest&& request) {
    method = request.method;
    dstHost = std::move(request.dstHost);
    dstPort = request.dstPort;
    urn = std::move(request.urn);
    headers = std::move(request.headers);
    content = std::move(request.content);
    return *this;
}

HTTP::HttpRequest::~HttpRequest() {

}

static HTTP::HttpMethod parseMethod(std::string const& method) {
    if (method == "GET") {
        return HTTP::HTTP_GET;
    }
    if (method == "POST") {
        return HTTP::HTTP_POST;
    }
    if (method == "DELETE") {
        return HTTP::HTTP_DELETE;
    }
    return HTTP::HTTP_UNKNOWN;
}

static std::string retrieveString(std::string const& data, int& start, int& end, char symbol) {
    end = data.find(symbol, start);
    //check end != npos and...
    std::string sub = data.substr(start, end-start);
    start = end+1;
    return sub;
}

static bool headerIsEnded(std::string const& data, int start) {
    static char endSequence[3] = "\r\n";
    for (int i = 0; i < 2; ++i) {
        if (data[start+i] != endSequence[i]) {
            return false;
        }
    }
    return true;
}

HTTP::HttpRequest HTTP::parseHttpRequest(std::string const& data) {
    int start = 0, end = 0;
    HttpMethod method = parseMethod(retrieveString(data, start, end, ' '));
    std::string urn = retrieveString(data, start, end, ' ');
    retrieveString(data, start, end, '\n');
    
    std::string host;
    int port;
    std::vector<HttpHeader> headers;
    while (!headerIsEnded(data, start)) {
        std::string key = retrieveString(data, start, end, ':');
        ++start;
        std::string value = retrieveString(data, start, end, '\r');    
        ++start;
        if (key == "Host") {
            int semIdx = value.find(':');
            host = value.substr(0, semIdx);
            port = (semIdx == -1) ? 80 : std::stoi(value.substr(semIdx+1));
        }
        headers.emplace_back(std::move(key), std::move(value));
    }
    std::string content = data.substr(start+2);
    return HttpRequest(method, host, port, urn, headers, content);
}

std::string HTTP::HttpRequest::getDstHost() const {
    return dstHost;
}

int HTTP::HttpRequest::getDstPort() const {
    return dstPort;
}