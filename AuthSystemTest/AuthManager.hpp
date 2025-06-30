#pragma once
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>   
#include <iphlpapi.h>  
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "iphlpapi.lib")


class AuthManager {
public:
    static std::string authenticate(const std::wstring& userID);
};
