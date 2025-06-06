#include "Client.h"

#include "Arguments/ArgumentParser.h"

#include "Transport/ITransport.h"
#include "Transport/TcpTransport.h"

#include "Message/IMessageConvert.h"
#include "Message/TcpMessageConvert.h"

#include "Common/serverOptions.h"
#include "Common/clientContext.h"

#include "Command/ICommandConvert.h"
#include "Command/TcpCommandConvert.h"

#include "States/IState.h"
#include "States/StartState.h"

#include "Output/debug.h" // for printf_debug, Info, Error

#include <iostream>

#include <memory> // for std::unique_ptr, std::shared_ptr - to automatically manage memory of the object(s)

#include <atomic>  // for std::atomic - to safely check for termination signal
#include <csignal> // for signal handling
inline std::atomic<bool> terminateSignal(false); // global variable for Client class to check for termination (has to be atomic)
void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        terminateSignal = true;
    }
}

int main(int argc, char* argv[]) {

    // Register signal handler for SIGINT and SIGTERM
    std::signal(SIGINT, signalHandler);

    try {
        serverOptions options;

        ArgumentParser parser(argc, argv);
        parser.Parse(options);

        std::shared_ptr<ITransport>      transport = std::make_shared<TcpTransport>();
        std::shared_ptr<IMessageConvert> messageConvert = std::make_shared<TcpMessageConvert>();
        std::shared_ptr<ICommandConvert> commandConvert = std::make_shared<TcpCommandConvert>();

        // Depending on the transport protocol, create corresponding instances
        if (options.transportProtocol == transportProtocolType::TCP) {
            
            transport = std::make_shared<TcpTransport>();
            messageConvert = std::make_shared<TcpMessageConvert>();
            commandConvert = std::make_shared<TcpCommandConvert>();

        } else if (options.transportProtocol == transportProtocolType::UDP) {
            throw std::runtime_error("UDP transport is not implemented yet.");
        }
            
    
        ClientContext context;
        context.options = options;
        context.transport = transport;
        context.messageConvert = messageConvert;
        context.commandConvert = commandConvert;
        context.state = nullptr;
    
        std::shared_ptr<IState> startState = std::make_shared<StartState>(context);
    
        context.state = startState;
    
        Client client(context);
    
        client.Run();
    }
    catch(const std::exception& e) {
        Error(std::string(e.what()));
        return 1;
    }
    

    return 0;
}