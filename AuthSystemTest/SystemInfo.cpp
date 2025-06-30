#include "SystemInfo.hpp"
#include <windows.h>
#include <iphlpapi.h>
#include <sstream>
#include <iomanip>
#include <wincrypt.h>
#include <cstdio>    
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "crypt32.lib")

std::string getVolumeSerial() {
    DWORD serial = 0;
    GetVolumeInformationA("C:\\", nullptr, 0, &serial, nullptr, nullptr, nullptr, 0);
    printf("Volume Serial: %lu (hex: %08X)\n", serial, serial);
    return std::to_string(serial);
}

std::string getComputerName() {
    char buffer[256] = { 0 };
    DWORD size = sizeof(buffer);
    GetComputerNameA(buffer, &size);
    printf("Computer Name: %s\n", buffer);
    return std::string(buffer);
}

std::string getMAC() {
    IP_ADAPTER_INFO AdapterInfo[16];
    DWORD buflen = sizeof(AdapterInfo);
    if (GetAdaptersInfo(AdapterInfo, &buflen) != ERROR_SUCCESS) {
        printf("Falha ao obter MAC\n");
        return "000000000000";
    }
    BYTE* mac = AdapterInfo[0].Address;
    std::ostringstream oss;
    for (int i = 0; i < 6; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)mac[i];
    std::string macStr = oss.str();
    printf("MAC Address: %s\n", macStr.c_str());
    return macStr;
}

std::string sha256(const std::string& data) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[32];
    DWORD hashLen = 32;
    std::ostringstream oss;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        printf("Erro CryptAcquireContext\n");
        return "";
    }

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        CryptReleaseContext(hProv, 0);
        printf("Erro CryptCreateHash\n");
        return "";
    }

    if (!CryptHashData(hHash, (BYTE*)data.c_str(), data.size(), 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        printf("Erro CryptHashData\n");
        return "";
    }

    if (CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0)) {
        for (DWORD i = 0; i < hashLen; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    std::string hashStr = oss.str();
    printf("SHA256 Hash: %s\n", hashStr.c_str());
    return hashStr;
}

std::string SystemInfo::getHWID() {
    std::string raw = getMAC() + getVolumeSerial() + getComputerName();
    printf("Raw HWID input string: %s\n", raw.c_str());
    return sha256(raw);
}
