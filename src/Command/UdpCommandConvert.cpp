#include "UdpCommandConvert.h"
#include <sstream>
#include <stdexcept>
#include <regex>

std::string UdpCommandConvert::EncodeCommand(const Command& command) const {
    std::cout << "Encoding command: " << command << std::endl;
    return "";
}

Command UdpCommandConvert::DecodeCommand(const std::string& data) const {
    std::cout << "Decoding command from data: " << data << std::endl;
    Command emptyCommand;
    return emptyCommand;
}