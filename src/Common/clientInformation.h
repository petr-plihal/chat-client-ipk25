#ifndef CLIENT_INFORMATION_H
#define CLIENT_INFORMATION_H

#include <string>

struct clientInformation {
    std::string displayName = "unknown";
    std::string username = "unknown";
    std::string secret = "unknown";
};

#endif // CLIENT_INFORMATION_H