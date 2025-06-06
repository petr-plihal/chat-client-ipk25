#include "Client.h"

#include "Transport/ITransport.h"
#include "Message/IMessageConvert.h"
#include "Command/ICommandConvert.h"

#include "Output/debug.h"

#include <iostream>
#include <string> // for std::string - to read from stdin
#include <vector> // for std::vector - to iterate over multiple received messages

#include <poll.h> // for poll - allows to wait for events on file descriptors
#include <atomic> // for std::atomic - to safely check for termination signal

#define FILE_DESCRIPTOR_COUNT 2
#define POLL_TIMEOUT 1000 // Dictates time it takes for poll to return -> indirectly dictates how often the loop is executed

// TODO: separate into multiple methods later
void Client::Run() {

    extern std::atomic<bool> terminateSignal;

    // Establish connection (can be different for TCP/UDP)
    context.transport->EstablishConnection(context.options);

    // Descriptors for polling - socket and stdin
    struct pollfd fileDescriptors[FILE_DESCRIPTOR_COUNT];
    fileDescriptors[0].fd = context.transport->GetSocket();
    fileDescriptors[1].fd = STDIN_FILENO;
    fileDescriptors[0].events = POLLIN;
    fileDescriptors[1].events = POLLIN;

    // try catch  to ensure socket is closed, the exception is then re thrown for main.cpp
    try {
        // nullptr signals that the connection was closed and no further communication is possible
        while ( context.state != nullptr ) {

            int ret = poll(fileDescriptors, FILE_DESCRIPTOR_COUNT, POLL_TIMEOUT);

            // Error during poll
            if (ret == -1) {

                // Interrupt, and caused by Ctrl-C -> end communication and exit
                if (errno == EINTR && terminateSignal) {
                    // Interrupted by a signal, should continue, if it is ctrl-c, it will be detected with terminateSignal?
                    if (terminateSignal) {
                        printf_debug("Termination signal ctrl-c received.");

                        Message outgoingMessage;
                        outgoingMessage.type = MessageType::BYE;
                        outgoingMessage.displayName = context.clientInfo.displayName;
                    
                        context.transport->SendData(
                            context.messageConvert->EncodeMessage(outgoingMessage)
                        );

                        context.state = nullptr;
                        break;
                    }
                    continue;
                } else {
                    Error("Error during poll");
                    break;
                }
            }

            // Check if the termination signal was received
            if (terminateSignal) {
                printf_debug("Termination signal received.");

                Message outgoingMessage;
                outgoingMessage.type = MessageType::BYE;
                outgoingMessage.displayName = context.clientInfo.displayName;

                context.transport->SendData(
                    context.messageConvert->EncodeMessage(outgoingMessage)
                );

                context.state = nullptr;
                break;
            }

            // Data is available on the socket
            // Socket will be handled first, before stdin
            if (fileDescriptors[0].revents & POLLIN) {
                std::string data = context.transport->ReceiveData();

                if (!data.empty()) {

                    // If incomplete message is received, loop will not execute
                    std::vector<Message> messages = context.messageConvert->DecodeStream(data);
                    for (const auto& msg : messages) {
                        context.state->HandleMessage(msg);
                    }

                }
            }

            // Data is available on stdin
            if (fileDescriptors[1].revents & POLLIN) {

                std::string commandString;

                if (!std::getline(std::cin, commandString)) {
                    printf_debug("EOF (Ctrl+D) detected, terminating.");

                    // Send BYE message to the server
                    Message outgoingMessage;
                    outgoingMessage.type = MessageType::BYE;
                    outgoingMessage.displayName = context.clientInfo.displayName;
                    context.transport->SendData(
                        context.messageConvert->EncodeMessage(outgoingMessage)
                    );
                    context.state = nullptr; // Is not needed if break is used, but for clarity

                    break;
                }

                // NOTE: sanity check
                if (commandString.empty()) {
                    continue;
                }

                Command incomingCommand = context.commandConvert->DecodeCommand(commandString);

                context.state->HandleCommand(incomingCommand);
            }
        }
    }
    catch (const std::exception& e) {

        // Send ERR message to the server
        Message outgoingMessage;
        outgoingMessage.type = MessageType::ERR;
        outgoingMessage.displayName = context.clientInfo.displayName;
        outgoingMessage.messageContent = e.what();

        context.transport->SendData(
            context.messageConvert->EncodeMessage(outgoingMessage)
        );

        context.transport->CloseSocket();
        throw; // Rethrow the exception to be handled in main.cpp
    }

    context.transport->CloseSocket();

}

