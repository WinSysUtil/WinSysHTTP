#include "HTTPAPI.h"

void HTTP_API::Create_HTTP(CRequest** ppObj, wchar_t * url)
{
    //(*ppObj) = new CRequest(url);

    return void();
}

void HTTP_API::Delete_HTTP(CRequest** ppObj)
{
    delete (*ppObj);
    (*ppObj) = nullptr;

    return void();
}
