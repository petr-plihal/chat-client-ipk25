#include "TcpMessageConvert.h"
#include <sstream> // For std::stringstream

#include <regex> // For std::regex - regular expressions
#include <stdexcept> // For std::invalid_argument - exception handling

#include <iostream> // For std::cerr - error output

std::string TcpMessageConvert::EncodeMessage(const Message& message) {

    // stringstream is more convenient for building simple strings in fewer lines
    // https://stackoverflow.com/questions/20594520/what-exactly-does-stringstream-do
    std::stringstream ss;

    switch (message.type) {
        case MessageType::AUTH:
            ss << "AUTH " << message.username << " AS " << message.displayName
               << " USING " << message.secret << "\r\n";
            break;

        case MessageType::JOIN:
            ss << "JOIN " << message.channelId << " AS " << message.displayName << "\r\n";
            break;

        case MessageType::MSG:
            ss << "MSG FROM " << message.displayName << " IS " << message.messageContent << "\r\n";
            break;

        case MessageType::ERR:
            ss << "ERR FROM " << message.displayName << " IS " << message.messageContent << "\r\n";
            break;

        case MessageType::REPLY:
            ss << "REPLY " << (message.replySuccess ? "OK" : "NOK") << " IS "
               << message.messageContent << "\r\n";
            break;

        case MessageType::BYE:
            ss << "BYE FROM " << message.displayName << "\r\n";
            break;

        default:
            throw std::invalid_argument("Unsupported message type for TCP encoding.");
    }

    return ss.str();
}

static bool contentLengthOk(const std::string& content) {
    return content.size() <= 60000;
}

Message TcpMessageConvert::DecodeMessage(const std::string& data) {
    Message message;
    std::smatch match;

    printf_debug("Decoding message: %s", EscapeNonPrintable(data).c_str());

    // AUTH message
    std::regex authRegex(R"(AUTH ([A-Za-z0-9_-]{1,20}) AS ([\x21-\x7E]{1,20}) USING ([A-Za-z0-9_-]{1,128})\r\n)", std::regex_constants::icase);
    if (std::regex_match(data, match, authRegex)) {
        message.type = MessageType::AUTH;
        message.username = match[1];
        message.displayName = match[2];
        message.secret = match[3];

        printf_debug("Decoded as AUTH message");

        return message;
    }

    // JOIN message
    std::regex joinRegex(R"(JOIN ([A-Za-z0-9_-]{1,20}) AS ([\x21-\x7E]{1,20})\r\n)", std::regex_constants::icase);
    if (std::regex_match(data, match, joinRegex)) {
        message.type = MessageType::JOIN;
        message.channelId = match[1];
        message.displayName = match[2];

        printf_debug("Decoded as JOIN message");

        return message;
    }

    // MSG message
    std::regex msgRegex(R"(MSG FROM ([\x21-\x7E]{1,20}) IS ([\x20-\x7E\x0A]+)\r\n)", std::regex_constants::icase);
    if (std::regex_match(data, match, msgRegex)) {
        message.type = MessageType::MSG;
        message.displayName = match[1];
        message.messageContent = match[2];

        if (contentLengthOk(message.messageContent)) {
            printf_debug("Decoded as MSG message");
            return message;
        }

    }

    // ERR message
    std::regex errRegex(R"(ERR FROM ([\x21-\x7E]{1,20}) IS ([\x20-\x7E\x0A]+)\r\n)", std::regex_constants::icase);
    if (std::regex_match(data, match, errRegex)) {
        message.type = MessageType::ERR;
        message.displayName = match[1];
        message.messageContent = match[2];

        if (contentLengthOk(message.messageContent)) {
            printf_debug("Decoded as ERR message");
            return message;
        }
    }

    // REPLY message
    std::regex replyRegex(R"(REPLY (OK|NOK) IS ([\x20-\x7E\x0A]+)\r\n)", std::regex_constants::icase);
    if (std::regex_match(data, match, replyRegex)) {
        message.type = MessageType::REPLY;
        std::string replyType = match[1];
        message.messageContent = match[2];

        // Make capital letters for case-insensitive comparison
        std::transform(
            replyType.begin(),
            replyType.end(), 
            replyType.begin(), 
            ::toupper
        );
    
        if ( 
            (replyType == "OK" || replyType == "NOK") && 
             contentLengthOk(message.messageContent) 
        ) {
            message.replySuccess = (replyType == "OK");
            printf_debug("Decoded as REPLY message");
            return message;
        }
    
    }

    // BYE message
    std::regex byeRegex(R"(BYE FROM ([\x21-\x7E]{1,20})\r\n)", std::regex_constants::icase);
    if (std::regex_match(data, match, byeRegex)) {
        message.type = MessageType::BYE;
        message.displayName = match[1];

        printf_debug("Decoded as BYE message");

        return message;
    }

    // If no regex matched, let the state handle the error
    message.type = MessageType::MALFORMED;

    printf_debug("Decoded as MALFORMED message");

    return message;
}

std::vector<Message> TcpMessageConvert::DecodeStream(const std::string& data) {
    std::vector<Message> messages;
    std::istringstream stream(incompleteMessage + data); // Add incomplete message to the stream first
    std::string line;

    incompleteMessage.clear(); // Avoid duplicate processing

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') { // Check for complete message
            try {
                messages.push_back(DecodeMessage(line + "\n")); // Add newline for proper decoding
            } catch (const std::invalid_argument& e) {
                // TODO: Handle/print invalid message format
                Error("Could not decode message: " + std::string(e.what()));
            }
        } else {
            // If the line is not complete, store it for later - incomplete message is always at the end of the stream
            incompleteMessage = line;
        }
    }

    return messages;
}