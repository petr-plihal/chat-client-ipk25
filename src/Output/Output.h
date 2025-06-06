#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <string>


inline void Info(const std::string& msg) {
    std::cout << msg << std::endl;
}

inline void Error(const std::string& msg) {
    std::cout << "ERROR: " << msg << std::endl;
}

// Output of /help command
inline void HelpForCommands() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  /auth <username> <secret> - Authenticate with the server" << std::endl;
    std::cout << "  /join <channelId> - Join a channel" << std::endl;
    std::cout << "  /rename <newDisplayName> - Rename your display name" << std::endl;
    std::cout << "  /help - Show this help message" << std::endl;
}


#endif // OUTPUT_H
