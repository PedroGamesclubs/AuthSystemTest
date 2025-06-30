#include "AuthManager.hpp"
#include "SystemInfo.hpp"
#include "StringUtils.hpp"
#include <wininet.h>
#include <cstdio>
#pragma comment(lib, "wininet.lib")

std::string AuthManager::authenticate(const std::wstring& userID) {
    std::string hwid = SystemInfo::getHWID();
    std::string postData = "id=" + StringUtils::urlEncode(std::string(userID.begin(), userID.end())) +
        "&hwid=" + StringUtils::urlEncode(hwid);

    HINTERNET hSession = InternetOpenA("AuthTeste", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    if (!hSession) return "Erro InternetOpen";

    HINTERNET hConnect = InternetConnectA(hSession, "141.11.128.63", INTERNET_DEFAULT_HTTP_PORT,
        nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        InternetCloseHandle(hSession);
        return "Erro InternetConnect";
    }

    const char* acceptTypes[] = { "application/json", nullptr };
    HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/api/auth",
        nullptr, nullptr, acceptTypes,
        INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_RELOAD, 0);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        return "Erro HttpOpenRequest";
    }

    const char* headers = "Content-Type: application/x-www-form-urlencoded\r\n";

    BOOL sent = HttpSendRequestA(hRequest, headers, (DWORD)strlen(headers),
        (LPVOID)postData.c_str(), (DWORD)postData.length());
    if (!sent) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        return "Erro HttpSendRequest";
    }

    char buffer[1024];
    DWORD bytesRead = 0;
    std::string response;

    while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = 0;
        response += buffer;
    }

    printf("Resposta do servidor: %s\n", response.c_str());

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);

    if (response.find("\"status\":\"ok\"") != std::string::npos) {
        return "Autenticado";
    }
    else {
        return "Falha na autenticação: " + response;
    }
}
