#pragma once

#include <string>
#include <vector>

class CRequest {
public:
    CRequest(const std::wstring& method, const std::wstring& url, const std::wstring& data, const std::vector<std::pair<std::wstring, std::wstring>>& headers)
        : method_(method), url_(url), data_(data), headers_(headers) {}

    const std::wstring& GetMethod() const {
        return method_;
    }

    const std::wstring& GetUrl() const {
        return url_;
    }

    const std::wstring& GetData() const {
        return data_;
    }

    const std::vector<std::pair<std::wstring, std::wstring>>& GetHeaders() const {
        return headers_;
    }

private:
    std::wstring method_;
    std::wstring url_;
    std::wstring data_;
    std::vector<std::pair<std::wstring, std::wstring>> headers_;
};
