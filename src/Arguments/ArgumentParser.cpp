#include "ArgumentParser.h"

#include "Output/Output.h"

#include <iostream>
#include <stdexcept>

#include <getopt.h> // for getopt
#include <netdb.h> // for gethostbyname

#include <arpa/inet.h> // for inet_pton - convert struct in_addr to string
// <arpa/inet.h> should also inclue AF_INET, so <sys/socket.h> should not be needed

void ArgumentParser::Parse(serverOptions& options) {
    
    // Default values should be set in the serverOptions struct when declared

    const char* short_opts = "t:s:p:d:r:h";

    const struct option long_opts[] = {
        {"transport",  required_argument, nullptr, 't'},
        {"server",     required_argument, nullptr, 's'},
        {"port",       required_argument, nullptr, 'p'},
        {"timeout",    required_argument, nullptr, 'd'},
        {"retransmit", required_argument, nullptr, 'r'},
        {"help",       no_argument,       nullptr, 'h'},
        {nullptr,      0,                 nullptr, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1) {
        switch (opt) {

            case 't': {
                std::string protocol = optarg;
                if (protocol == "tcp") {
                    options.transportProtocol = transportProtocolType::TCP;
                } else if (protocol == "udp") {
                    options.transportProtocol = transportProtocolType::UDP;
                } else {
                    throw std::invalid_argument("Invalid value for -t/--transport. Expected 'tcp' or 'udp'");
                }
                break;
            }
            case 's':
                options.serverHostname = optarg;
                options.serverAddress = ResolveHostname(options.serverHostname);
                break;
            case 'p': {

                int port = std::stoi(optarg);

                if (port < 1 || port > 65535) {
                    throw std::invalid_argument("Invalid port number - must be in range 1-65535");
                }
                
                options.serverPort = static_cast<uint16_t>(port);
                break;
            }
            case 'd':
                options.udpTimeout = static_cast<uint16_t>(std::stoi(optarg));
                break;
            case 'r':
                options.udpMaxRetransmit = static_cast<uint8_t>(std::stoi(optarg));
                break;
            case 'h':
                PrintHelp();
                exit(0);
            default:
                throw std::invalid_argument("Unknown argument");

        }
    }

    // Mandatory arguments
    if (options.transportProtocol == transportProtocolType::UNDEFINED) {
        throw std::invalid_argument("Missing mandatory argument: -t/--transport");
    }
    if (options.serverHostname.empty()) {
        throw std::invalid_argument("Missing mandatory argument: -s/--server");
    }
}

std::string ArgumentParser::ResolveHostname(const std::string& hostname) {

    struct in_addr addr;

    // Check if the hostname is an IP address (IPv4)
    // inet_aton could be used also, but inet_pton is supposed to be more versatile
    if (inet_pton(AF_INET, hostname.c_str(), &addr) == 1) {
        return hostname;
    }

    // USE getaddrinfo, NOT gethostbyname
    struct addrinfo hints = {};
    hints.ai_family = AF_INET; // IPv4 only
    hints.ai_socktype = SOCK_STREAM;

    // getaddrinfo will attempt to resolve the hostname to an IP address
    struct addrinfo* result;
    if (getaddrinfo(hostname.c_str(), nullptr, &hints, &result) != 0) {
        throw std::runtime_error("Failed to resolve hostname: " + hostname);
    }

    // Check if the result is valid
    char ip[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &((struct sockaddr_in*)result->ai_addr)->sin_addr, ip, sizeof(ip)) == nullptr) {
        freeaddrinfo(result);
        throw std::runtime_error("Failed to convert resolved address to IPv4: " + hostname);
    }

    freeaddrinfo(result);
    return std::string(ip);
}

void ArgumentParser::PrintHelp() const {
    std::cout << "Usage: program -t <tcp|udp> -s <server> [-p <port>] [-d <timeout>] [-r <retransmit>] [-h]\n"
              << "  -t <tcp|udp>       Transport protocol (mandatory)\n"
              << "  -s <server>        Server IP address or hostname (mandatory)\n"
              << "  -p <port>          Server port (default: 4567)\n"
              << "  -d <timeout>       UDP confirmation timeout in milliseconds (default: 250)\n"
              << "  -r <retransmit>    Maximum number of UDP retransmissions (default: 3)\n"
              << "  -h                 Print this help message and exit\n";
}