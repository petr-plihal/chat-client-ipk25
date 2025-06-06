#include "TcpCommandConvert.h"

#include <sstream>
#include <stdexcept>
#include <regex>
#include <iostream>
#include <string>

std::string TcpCommandConvert::EncodeCommand(const Command& command) const {
    std::stringstream ss;

    switch (command.type) {
        case CommandType::AUTH:
            ss << "AUTH " << command.username << " USING " << command.secret << "\r\n";
            break;
        case CommandType::JOIN:
            ss << "JOIN " << command.channelId << " AS " << command.displayName << "\r\n";
            break;
        case CommandType::RENAME:
            ss << "RENAME TO " << command.displayName << "\r\n";
            break;
        case CommandType::HELP:
            ss << "HELP\r\n";
            break;
        case CommandType::MESSAGE:
            ss << "MESSAGE " << command.messageContent << "\r\n";
            break;
        default:
            throw std::invalid_argument("Internal error: Unsupported command type for TCP encoding.");
    }

    return ss.str();
}

Command TcpCommandConvert::DecodeCommand(const std::string& data) const {
    Command command;

    // Regex for valid command prefix
    std::regex command_prefix_regex("^/([a-z0-9_-]+)");
    std::smatch match;

    // Check if the command starts with a valid prefix
    // TODO: 
    if (std::regex_search(data, match, command_prefix_regex)) {

        std::string commandName = match[1]; // Extrahovaný název příkazu

        // Další regulární výrazy pro konkrétní příkazy
        std::regex auth_regex("^/auth\\s+([a-zA-Z0-9_-]+)\\s+([a-zA-Z0-9_-]+)\\s+([a-zA-Z0-9_-]+)$");
        std::regex join_regex("^/join\\s+([a-zA-Z0-9_-]+)$");
        std::regex rename_regex("^/rename\\s+([a-zA-Z0-9_-]+)$");
        std::regex help_regex("^/help$");

        if (std::regex_match(data, match, auth_regex)) {
            command.type = CommandType::AUTH;
            command.username = match[1];
            command.secret = match[2];
            command.displayName = match[3];
        } else if (std::regex_match(data, match, join_regex)) {
            command.type = CommandType::JOIN;
            command.channelId = match[1];
        } else if (std::regex_match(data, match, rename_regex)) {
            command.type = CommandType::RENAME;
            command.displayName = match[1];
        } else if (std::regex_match(data, help_regex)) {
            command.type = CommandType::HELP;
        } else {
            printf_debug("Unknown command read, setting: %s", data.c_str());
            // NOTE: Unknown command should be handled in the state
            command.type = CommandType::INVALID;
            command.messageContent = data;
        }
    } else {

        // If the command does not start with a valid prefix, treat it as a message
        command.type = CommandType::MESSAGE;
        command.messageContent = data;

    }

    return command;

}

