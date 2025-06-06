#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>

enum class MessageType {
    /* Default value - used for uninitialized messages.
       It means internal class did not handle the message correctly -> error + terminate the program communication in current state. */
    UNDEFINED,

    AUTH,
    BYE,
    CONFIRM,
    ERR,
    JOIN,
    MSG,
    PING,
    REPLY,

    /* Incoming message from server did not match the expected format.
       -> error + terminate the program communication in current state. */
    MALFORMED,
};

struct Message {
    MessageType type;
    std::string username;
    std::string secret;
    std::string displayName;
    std::string channelId;
    std::string messageContent;
    bool replySuccess;

};

inline std::string MessageTypeToString(MessageType type) {
    switch (type) {
        case MessageType::UNDEFINED: return "UNDEFINED";
        case MessageType::AUTH: return "AUTH";
        case MessageType::BYE: return "BYE";
        case MessageType::CONFIRM: return "CONFIRM";
        case MessageType::ERR: return "ERR";
        case MessageType::JOIN: return "JOIN";
        case MessageType::MSG: return "MSG";
        case MessageType::PING: return "PING";
        case MessageType::REPLY: return "REPLY";
        default: return "UNKNOWN";
    }
}

inline std::ostream& operator<<(std::ostream& os, const Message& message) {
    os << "Message {" << std::endl
       << "    type: " << MessageTypeToString(message.type) << std::endl
       << "    username: " << message.username << std::endl
       << "    secret: " << message.secret << std::endl
       << "    displayName: " << message.displayName << std::endl
       << "    channelId: " << message.channelId << std::endl
       << "    messageContent: " << message.messageContent << std::endl
       << "    replySuccess: " << (message.replySuccess ? "true" : "false") << std::endl
       << " }" << std::endl
       ;
    return os;
}

#endif // MESSAGE_H