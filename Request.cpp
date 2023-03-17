#include "Request.h"

Request::Request(std::wstring url) : url(url) {
    hSession = WinHttpOpen(L"WinHTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    hConnect = WinHttpConnect(hSession, url.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
}

void Request::SetHeader(std::wstring header) {
    headers.push_back(header);
}

void Request::SetData(std::wstring data) {
    this->data = data;
}

std::unique_ptr<Response> Request::GET()
{
    return Send(L"GET");
}

std::unique_ptr<Response> Request::POST()
{
    return Send(L"POST");
}

std::unique_ptr<Response> Request::PATCH()
{
    return Send(L"PATCH");
}

std::unique_ptr<Response> Request::PUT()
{
    return Send(L"PUT");
}

std::unique_ptr<Response> Request::DELETE()
{
    return Send(L"DELETE");

}

std::unique_ptr<Response> Request::Send(std::wstring method) 
{
    hRequest = WinHttpOpenRequest(hConnect, method.c_str(), url.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    for (auto& header : headers) {
        WinHttpAddRequestHeaders(hRequest, header.c_str(), header.length(), WINHTTP_ADDREQ_FLAG_ADD);
    }
    WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, const_cast<wchar_t*>(data.c_str()), data.length(), data.length(), NULL);
    WinHttpReceiveResponse(hRequest, NULL);

    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    std::wstring response;

    do {
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
            break;
        }

        wchar_t* pszOutBuffer = new wchar_t[dwSize + 1];
        ZeroMemory(pszOutBuffer, (dwSize + 1) * sizeof(wchar_t));

        if (WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
            response += pszOutBuffer;
        }

        delete[] pszOutBuffer;
    } while (dwSize > 0);

    auto responsePtr = std::make_unique<Response>(response);
    return responsePtr;
}
