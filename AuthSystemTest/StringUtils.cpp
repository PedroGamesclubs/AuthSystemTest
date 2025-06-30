#include "StringUtils.hpp"
#include <sstream>
#include <iomanip>

std::string StringUtils::urlEncode(const std::string& str) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (unsigned char c : str) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            escaped << c;
        else
            escaped << '%' << std::setw(2) << (int)c;
    }

    return escaped.str();
}
