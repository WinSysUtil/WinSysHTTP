#pragma once

#include <windows.h>
#include <memory>
#include <winhttp.h>
#include <string>
#include <vector>
#pragma hdrstop

#include "Response.h"

#ifdef DELETE
#undef DELETE
#endif

class Request {
public:
    Request(std::wstring url);

    virtual void SetHeader(std::wstring header);
    virtual void SetData(std::wstring data);
    virtual std::unique_ptr<Response> GET();
    virtual std::unique_ptr<Response> POST();
    virtual std::unique_ptr<Response> PATCH();
    virtual std::unique_ptr<Response> PUT();
    virtual std::unique_ptr<Response> DELETE();

private:
    virtual std::unique_ptr<Response> Send(std::wstring method);

private:
    std::wstring url;
    std::vector<std::wstring> headers;
    std::wstring data;
    HINTERNET hSession = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;
};