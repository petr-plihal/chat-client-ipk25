#ifndef SERVER_OPTIONS_H
#define SERVER_OPTIONS_H

#include <string> // for std::string
#include <cstdint> // for uint16_t and uint8_t

#include <ostream> // for std::ostream

enum class transportProtocolType {
    UNDEFINED, // default - used for uninitialized
    
    TCP,
    UDP,
};

struct serverOptions {
    std::string serverHostname;
    std::string serverAddress;
    uint16_t serverPort = 4567;
    transportProtocolType transportProtocol;
    uint16_t udpTimeout = 250; // in milliseconds
    uint8_t udpMaxRetransmit = 3;
};

inline std::ostream& operator<<(std::ostream& os, const serverOptions& options) {
    os << "Server Hostname: " << options.serverHostname << std::endl
       << "Server Address: " << options.serverAddress << std::endl
       << "Server Port: " << options.serverPort << std::endl
       << "Transport Protocol: "
       << (options.transportProtocol == transportProtocolType::TCP ? "TCP" :
           options.transportProtocol == transportProtocolType::UDP ? "UDP" : "UNDEFINED")
       << std::endl
       << "UDP Timeout: " << options.udpTimeout << " ms\n"
       << "UDP Max Retransmit: " << static_cast<int>(options.udpMaxRetransmit) << std::endl
       ;
    return os;
}

#endif // SERVER_OPTIONS_H