﻿#include "HttpConnect.h"

CHttpConnect::CHttpConnect() {
    session_ = WinHttpOpen(L"CHttpConnect::CHttpConnect", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session_) {
        throw std::runtime_error("Failed to open session");
    }
}

CHttpConnect::~CHttpConnect() {
    WinHttpCloseHandle(session_);
}

CResponse CHttpConnect::SendRequest(const CRequest& request) {
    // Crack the URL into its components
    URL_COMPONENTS urlComponents = {};
    urlComponents.dwStructSize = sizeof(urlComponents);

    std::wstring hostNameBuf(256, L'\0');
    urlComponents.lpszHostName = &hostNameBuf[0];
    urlComponents.dwHostNameLength = static_cast<DWORD>(hostNameBuf.size());

    std::wstring urlPathBuf(1024, L'\0');
    urlComponents.lpszUrlPath = &urlPathBuf[0];
    urlComponents.dwUrlPathLength = static_cast<DWORD>(urlPathBuf.size());

    std::wstring extraInfoBuf(256, L'\0');
    urlComponents.lpszExtraInfo = &extraInfoBuf[0];
    urlComponents.dwExtraInfoLength = static_cast<DWORD>(extraInfoBuf.size());

    if (!WinHttpCrackUrl(request.GetUrl().c_str(), static_cast<DWORD>(request.GetUrl().length()), 0, &urlComponents)) {
        throw std::runtime_error("Failed to crack URL");
    }

    // Create the connection
    HINTERNET connect = WinHttpConnect(session_, hostNameBuf.c_str(), urlComponents.nPort, 0);
    if (!connect) {
        throw std::runtime_error("Failed to connect");
    }

    // Create the request
    const wchar_t* accept_types[] = { L"*/*", nullptr };
    HINTERNET httpRequest = WinHttpOpenRequest(connect, request.GetMethod().c_str(), urlPathBuf.c_str(), NULL, WINHTTP_NO_REFERER, accept_types, urlComponents.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (!httpRequest) {
        WinHttpCloseHandle(connect);
        throw std::runtime_error("Failed to create request");
    }

    // Set the headers
    for (const auto& header : request.GetHeaders()) {
        std::wstring wstrHeader;
        wstrHeader.append(header.first);
        wstrHeader.append(L": ");
        wstrHeader.append(header.second);
        wstrHeader.append(L"\r");

        if (!WinHttpAddRequestHeaders(httpRequest, wstrHeader.c_str(), wstrHeader.length(), WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE)) {
            WinHttpCloseHandle(httpRequest);
            WinHttpCloseHandle(connect);
            throw std::runtime_error("Failed to add request header");
        }
    }

    auto nLenBody = static_cast<DWORD>(request.GetData().length() * sizeof(wchar_t));
    if (!WinHttpSendRequest(httpRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, nLenBody, 0))
    {
        WinHttpCloseHandle(httpRequest);
        WinHttpCloseHandle(connect);
        throw std::runtime_error("Failed to send request");
    }


    if (nLenBody > 0) {
        DWORD bytes_written = 0;
        if (!WinHttpWriteData(httpRequest, request.GetData().c_str(), nLenBody, &bytes_written))
        {
            DWORD err = ::GetLastError();
            throw std::runtime_error("WinHttpWriteData() failed");
        }
        if (bytes_written != nLenBody) {
            throw std::runtime_error("WinHttpWriteData did not send entire request_t body");
        }

    }

    // Wait for the response
    if (!WinHttpReceiveResponse(httpRequest, nullptr)) {
        WinHttpCloseHandle(httpRequest);
        WinHttpCloseHandle(connect);
        throw std::runtime_error("Failed to receive response");
    }

    // Read the response
    std::vector<char> buffer(1024);
    std::string strResponseBody;
    DWORD bytesRead = 0;
    while (WinHttpReadData(httpRequest, &buffer[0], buffer.size(), &bytesRead) && bytesRead > 0) {
        strResponseBody.append(buffer.begin(), buffer.begin() + bytesRead);
    }


    // Get the status code
    DWORD statusCode = 0;
    DWORD statusCodeSize = sizeof(statusCode);
    if (!WinHttpQueryHeaders(httpRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX)) {
        WinHttpCloseHandle(httpRequest);
        WinHttpCloseHandle(connect);
        throw std::runtime_error("Failed to get status code");
    }

    // Get the headers
    std::map<std::wstring, std::wstring> responseHeaders;
    DWORD dwSize = 0;
    BOOL bResults = WinHttpQueryHeaders(
        httpRequest,
        WINHTTP_QUERY_RAW_HEADERS_CRLF,
        WINHTTP_HEADER_NAME_BY_INDEX,
        NULL,
        &dwSize,
        WINHTTP_NO_HEADER_INDEX);

    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        std::wstring headers(dwSize / sizeof(wchar_t), L'\0');

        bResults = WinHttpQueryHeaders(
            httpRequest,
            WINHTTP_QUERY_RAW_HEADERS_CRLF,
            WINHTTP_HEADER_NAME_BY_INDEX,
            &headers[0],
            &dwSize,
            WINHTTP_NO_HEADER_INDEX);

        if (bResults) {
            std::wstringstream ss(headers);
            std::wstring headerName, headerValue;
            while (std::getline(ss, headerName, L':') && std::getline(ss, headerValue)) {
                headerValue.erase(0, headerValue.find_first_not_of(L' '));
                headerValue.erase(headerValue.find_last_not_of(L' ') + 1);
                responseHeaders[headerName] = headerValue;
            }
        }
        else {
            // Handle error
        }
    }
    else {
        // Handle error
    }



    DWORD dwErr = GetLastError();

    WinHttpCloseHandle(httpRequest);
    WinHttpCloseHandle(connect);

    int size = MultiByteToWideChar(CP_UTF8, 0, strResponseBody.c_str(), -1, nullptr, 0);
    std::wstring wstrResponseBody(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, strResponseBody.c_str(), -1, &wstrResponseBody[0], size);


    return CResponse(statusCode, std::vector<std::pair<std::wstring, std::wstring>>(responseHeaders.begin(), responseHeaders.end()), wstrResponseBody);
}