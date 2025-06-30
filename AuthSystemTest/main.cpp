#include <iostream>
#include <string>
#include "AuthManager.hpp"

int main() {
    std::wstring userID;
    std::wcout << L"Digite seu ID: ";
    std::getline(std::wcin, userID);

    std::string resultado = AuthManager::authenticate(userID);
    std::cout << "Status da autenticação: " << resultado << std::endl;

    std::cout << "Pressione Enter para sair...";
    std::cin.get();

    return 0;
}
