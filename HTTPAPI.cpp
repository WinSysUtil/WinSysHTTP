#include "HTTPAPI.h"

void HTTP_API::Create_HTTP(Request** ppObj, wchar_t * url)
{
    (*ppObj) = new Request(url);

    return void();
}

void HTTP_API::Delete_HTTP(Request** ppObj)
{
    delete (*ppObj);
    (*ppObj) = nullptr;

    return void();
}
