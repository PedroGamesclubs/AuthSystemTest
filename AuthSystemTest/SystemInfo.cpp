#include "SystemInfo.hpp"
#include <windows.h>
#include <iphlpapi.h>
#include <sstream>
#include <iomanip>
#include <wincrypt.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "crypt32.lib")

std::string getVolumeSerial() {
    DWORD serial = 0;
    GetVolumeInformationA("C:\\", nullptr, 0, &serial, nullptr, nullptr, nullptr, 0);
    return std::to_string(serial);
}

std::string getComputerName() {
    char buffer[256];
    DWORD size = sizeof(buffer);
    GetComputerNameA(buffer, &size);
    return std::string(buffer);
}

std::string getMAC() {
    IP_ADAPTER_INFO AdapterInfo[16];
    DWORD buflen = sizeof(AdapterInfo);
    GetAdaptersInfo(AdapterInfo, &buflen);
    BYTE* mac = AdapterInfo[0].Address;
    std::ostringstream oss;
    for (int i = 0; i < 6; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)mac[i];
    return oss.str();
}

std::string sha256(const std::string& data) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE hash[32];
    DWORD hashLen = 32;
    std::ostringstream oss;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        return "";
    }

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        CryptReleaseContext(hProv, 0);
        return "";
    }

    if (!CryptHashData(hHash, (BYTE*)data.c_str(), data.size(), 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "";
    }

    if (CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0)) {
        for (DWORD i = 0; i < hashLen; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return oss.str();
}

std::string SystemInfo::getHWID() {
    std::string raw = getMAC() + getVolumeSerial() + getComputerName();
    return sha256(raw);
}
