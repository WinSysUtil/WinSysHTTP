#pragma once

#include <string>
#include <vector>

class CResponse {
public:
    CResponse(int status_code, const std::vector<std::pair<std::wstring, std::wstring>>& headers, const std::wstring& body)
        : status_code_(status_code), headers_(headers), body_(body) {}

    virtual int GetStatusCode() const {
        return status_code_;
    }

    virtual const std::vector<std::pair<std::wstring, std::wstring>>& GetHeaders() const {
        return headers_;
    }

    virtual const std::wstring& GetBody() const {
        return body_;
    }

private:
    int status_code_;
    std::vector<std::pair<std::wstring, std::wstring>> headers_;
    std::wstring body_;
};
