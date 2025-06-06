#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <iostream>

enum class CommandType {
    /* Default - used for uninitialized commands
       It means internal class did not handle the command correctly -> error + terminate the program in current state */
    UNDEFINED,
    
    AUTH,   
    JOIN, 
    RENAME, 
    HELP,   
    MESSAGE,

    /* Invalid command = anything that starts with /, but is not a valid command.
       It could mean a typo or a command that is not implemented yet.
       -> error, but don't exit the application */
    INVALID,
};

struct Command {
    CommandType type;
    std::string username;
    std::string secret;
    std::string displayName;
    std::string channelId;
    std::string messageContent;
};

inline std::string CommandTypeToString(CommandType type) {
    switch (type) {
        case CommandType::UNDEFINED: return "UNDEFINED";
        case CommandType::AUTH: return "AUTH";
        case CommandType::JOIN: return "JOIN";
        case CommandType::RENAME: return "RENAME";
        case CommandType::HELP: return "HELP";
        case CommandType::MESSAGE: return "MESSAGE";
        default: return "UNKNOWN";
    }
}

inline std::ostream& operator<<(std::ostream& os, const Command& command) {
    os << "Command {" << std::endl
       << "    type: " << CommandTypeToString(command.type) << std::endl
       << "    username: " << command.username << std::endl
       << "    secret: " << command.secret << std::endl
       << "    displayName: " << command.displayName << std::endl
       << "    channelId: " << command.channelId << std::endl
       << "    messageContent: " << command.messageContent << std::endl
       << " }" << std::endl;
    return os;
}

#endif // COMMAND_H