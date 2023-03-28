#pragma once
#include "Request.h"

#if WINSYSHTTP_EXPORTS
#define WINSYSHTTP_API _declspec(dllexport)
#else
#define WINSYSHTTP_API _declspec(dllexport)
#endif

namespace HTTP_API
{
	typedef void(*fp_Create_HTTP)(CRequest** ppObj, wchar_t* url);
	typedef void(*fp_Delete_HTTP)(CRequest** ppObj);

	/**
	 * @brief 새 HTTP 요청 객체를 생성합니다.
	 * @param ppObj Request 객체를 가리키는 포인터의 포인터입니다.
	 * @param url 요청을 보낼 URL입니다.
	 */
	WINSYSHTTP_API void Create_HTTP(CRequest** ppObj, wchar_t* url);
	/**
	 * @brief HTTP 요청 객체를 삭제합니다.
	 * @param ppObj Request 객체를 가리키는 포인터의 포인터입니다.
	 */
	WINSYSHTTP_API void Delete_HTTP(CRequest** ppObj);
}
