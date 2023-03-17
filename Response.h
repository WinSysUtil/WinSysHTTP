#pragma once

#include <windows.h>
#include <winhttp.h>
#include <string>
#include <vector>
#pragma hdrstop

#pragma comment(lib, "WinHTTP.lib")

class Response {
public:
    Response(std::wstring response);

    virtual std::wstring GetStatus();
    virtual std::wstring GetHeader(std::wstring name);
    virtual std::wstring GetBody();

private:
    std::wstring response;
    std::wstring status;
    std::vector<std::pair<std::wstring, std::wstring>> headers;
    std::wstring body;

    virtual void ParseStatusLine();
    virtual void ParseHeaders();
    virtual void ParseBody();
};