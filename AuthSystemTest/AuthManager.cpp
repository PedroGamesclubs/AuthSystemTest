#include "AuthManager.hpp"
#include "SystemInfo.hpp"
#include "StringUtils.hpp"
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

std::string AuthManager::authenticate(const std::wstring& userID) {
    std::string hwid = SystemInfo::getHWID();
    std::string postData = "id=" + StringUtils::urlEncode(std::string(userID.begin(), userID.end())) +
        "&hwid=" + StringUtils::urlEncode(hwid);

    HINTERNET hSession = InternetOpenA("AuthTeste", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (!hSession) return "Erro InternetOpen";

    HINTERNET hConnect = InternetOpenUrlA(hSession,
        "http://141.11.128.63:8080/api/auth", nullptr, 0, INTERNET_FLAG_RELOAD, 0);

    if (!hConnect) {
        InternetCloseHandle(hSession);
        return "Erro ao conectar";
    }

    char buffer[1024];
    DWORD bytesRead;
    std::string response;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = 0;
        response += buffer;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);

    if (response.find("ok") != std::string::npos) return "Autenticado";
    return "Falha na autenticação";
}
