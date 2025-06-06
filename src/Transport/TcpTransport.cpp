#include "TcpTransport.h"

#include <sys/socket.h> // For socket functions
#include <unistd.h>     // For close function
#include <netinet/in.h> // For sockaddr_in - IPv4 address
#include <arpa/inet.h>  // For inet_addr function - convert string to binary form
#include <fcntl.h>      // For fcntl function - set socket options for non-blocking mode
#include <poll.h>       // For poll - it allows to wait for 3 way handshake to be completed
#include <iomanip>      // For std::hex and std::setw
#include <sstream>      // For std::ostringstream

#include <cstring> // For strerror function - get error message from errno
#include <iostream>

#define POLL_SOCKET_TIMEOUT 10000 // Timeout for poll in milliseconds -> 10 seconds
#define RECEIVED_DATA_BUFFER_SIZE 1024 // Buffer size for received data -> 1024 bytes

TcpTransport::~TcpTransport() {
    CloseSocket();
}

void TcpTransport::EstablishConnection(const serverOptions& options) {

    this->socketDescriptor = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket for IPv4
    if (this->socketDescriptor < 0) {
        throw std::runtime_error("Error creating socket.");
    }

    printf_debug("Socket created with number: %d", this->socketDescriptor);

    // NOTE: This binding is redundant for client, OS should automatically bind to a free port
    // bind(this->socketDescriptor, nullptr, 0);

    // NOTE: Checksum seems to be incorrect, but it doesnt seem to impact functionality 

    // Get current socket flags, to ensure no already set flags are overwritten
    int flags = fcntl(this->socketDescriptor, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error("Error getting socket flags.");
    }

    // Set socket to non-blocking mode -> will not wait for data to be available, if none already are in the buffer -> returns immediately
    if (fcntl(this->socketDescriptor, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("Error setting socket to non-blocking mode.");
    }

    // Create server address structure
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr)); // Clear the structure

    // Set address family, port and address
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(options.serverPort);

    // Convert server address from string to binary form using inet_pton intead of inet_addr
    if (inet_pton(AF_INET, options.serverAddress.c_str(), &servAddr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid server address: " + options.serverAddress);
    }

    // Connect to the server - since socket is non-blocking, this will return immediately, before the connection could even be established
    // -> use poll to wait for the connection to be established
    printf_debug("Connecting to server at %s:%d", options.serverAddress.c_str(), options.serverPort);

    int connectionResult = connect(this->socketDescriptor, (struct sockaddr*)&servAddr, sizeof(servAddr));

    // connectionResult < 0 could mean that the connection is just in progress (EINPROGRESS) -> check errno for !EINPROGRESS
    if ( (connectionResult < 0) && (errno != EINPROGRESS) ) {
        throw std::runtime_error("Error connecting to server.");
    }

    // Non blocking scoket will return immediately, even if the connection is not established yet.
    // If the connection is in progress, poll will wait for the socket to be writable
    // NOTE: https://stackoverflow.com/questions/37011195/how-to-poll-simultaneously-from-stdin-and-from-sockets
    if (connectionResult < 0 && errno == EINPROGRESS) {

        struct pollfd fileDescriptor;
        fileDescriptor.fd = this->socketDescriptor;
        fileDescriptor.events = POLLOUT; // Wait for the socket to be writable, if it is, the poll will return

        int pollResult = poll(&fileDescriptor, 1, POLL_SOCKET_TIMEOUT);
        if (pollResult == -1) {
            throw std::runtime_error("Error polling socket.");
        } else if (pollResult == 0) {
            throw std::runtime_error("Connection timed out.");
        }
    }

    printf_debug("Connected to server at %s:%d", options.serverAddress.c_str(), options.serverPort);

}

void TcpTransport::SendData(const std::string& data) {

    /*
        Tcp is stream oriented, so the data is not sent as a single message, but as a stream of bytes.
        So, sometimes, only part of the message may be sent at once, and the rest may be sent later.
        Therefore the message may have to be sent in multiple iterations.
    */

    printf_debug("Will try to send data: %s", EscapeNonPrintable(data).c_str());

    size_t totalBytesSent = 0;
    size_t dataSize = data.size();

    while (totalBytesSent < dataSize) {

        ssize_t bytesSent = 
            send(
                this->socketDescriptor,
                data.c_str() + totalBytesSent, // Pointer to the data to send
                dataSize - totalBytesSent,     // Remaining data to send
                0                              // No flags
            );

        if (bytesSent == -1) {
            
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Socket is not ready for sending, wait and retry
                continue;
            } else {
                throw std::runtime_error("Error sending data: " + std::string(strerror(errno)));
            }
        }
        totalBytesSent += bytesSent;
    }

    printf_debug("Sent %zd bytes: %s", totalBytesSent, EscapeNonPrintable(data).c_str());

}

std::string TcpTransport::ReceiveData() {
    
    // NOTE: Possible return values of recv: https://pubs.opengroup.org/onlinepubs/007904975/functions/recv.html

    char buffer[RECEIVED_DATA_BUFFER_SIZE]; // Buffer to store received data
    ssize_t bytesRead = 
        recv(
            this->socketDescriptor,
            buffer,                 // Pointer to the buffer to store received data
            sizeof(buffer) - 1,     // Size of the buffer - 1 for null terminator '\0'
            0                       // No flags
        );

    if (bytesRead < 0) {

        // No data available, return empty string
        if ( (errno == EAGAIN) || (errno == EWOULDBLOCK) ) { return ""; }

        // Other errors
        throw std::runtime_error("Error receiving data: " + std::string(strerror(errno)));

    } else if (bytesRead == 0) {
        throw std::runtime_error("Connection closed by server.");
    }

    std::string receivedData(buffer, bytesRead); // Create a string from the buffer - no need to add '\0'

    printf_debug("Received %zd bytes: %s", bytesRead, EscapeNonPrintable(receivedData).c_str());
    //std::cout << "Received " << bytesRead << " bytes: " << EscapeNonPrintable(receivedData) << std::endl;

    return receivedData; // Return the received data
}

void TcpTransport::CloseSocket() {

    if (this->socketDescriptor >= 0) {

        close(this->socketDescriptor);
        this->socketDescriptor = -1;

        printf_debug("Socket closed.");
    }

}
