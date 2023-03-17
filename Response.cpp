#include "Response.h"

Response::Response(std::wstring response) : response(response) {
    ParseStatusLine();
    ParseHeaders();
    ParseBody();
}

std::wstring Response::GetStatus() {
    return status;
}

std::wstring Response::GetHeader(std::wstring name) {
    for (auto& header : headers) {
        if (header.first == name) {
            return header.second;
        }
    }
    return L"";
}

std::wstring Response::GetBody() {
    return body;
}

void Response::ParseStatusLine() {
    size_t pos = response.find(L"\\r\\n");
    status = response.substr(0, pos);
    response.erase(0, pos + 2);
}

void Response::ParseHeaders() {
    size_t pos;
    while ((pos = response.find(L"\\r\\n")) != std::wstring::npos && pos != 0) {
        std::wstring header = response.substr(0, pos);
        response.erase(0, pos + 2);
        size_t colonPos = header.find(L": ");
        headers.push_back(std::make_pair(header.substr(0, colonPos), header.substr(colonPos + 2)));
    }
}

void Response::ParseBody() {
    body = response;
}