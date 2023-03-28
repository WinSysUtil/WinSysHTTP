#pragma once
#include <Windows.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <map>

#include <winhttp.h>
#pragma comment(lib, "WinHTTP.lib")
#pragma hdrstop

#include "Request.h"
#include "Response.h"

/*
int main() {
    // Create an instance of WinHttpWrapper
    CHttpConnect wrapper;

    // Send a GET request to example.com and print the response
    std::wstring url = L"https://jsonplaceholder.typicode.com/posts/1";
    CRequest request(L"GET", url, L"", {});
    CResponse response = wrapper.SendRequest(request);

    std::cout << "Status code: " << response.GetStatusCode() << std::endl;
    std::cout << "Headers:" << std::endl;
    for (const auto& header : response.GetHeaders()) {
        std::wcout << header.first << L": " << header.second << std::endl;
    }

    std::wcout << L"Response body: " << response.GetBody() << std::endl;

    return 0;
}


*/

#ifdef WINSYSHTTP_EXPORTS
#define WINSYSHTTP_API __declspec(dllexport) 
#else
#define WINSYSHTTP_API __declspec(dllimport) 
#endif

class WINSYSHTTP_API CHttpConnect
{
public:
    CHttpConnect();

    virtual ~CHttpConnect();

    virtual CResponse SendRequest(const CRequest& request);

private:
    HINTERNET session_;
};