#ifndef CLIENTCONTEXT_H
#define CLIENTCONTEXT_H

#include "Common/serverOptions.h"

// #include "States/IState.h"
// Would cause circular dependency -> use forward declaration
class IState;

#include "Transport/ITransport.h"
#include "Message/IMessageConvert.h"
#include "Command/ICommandConvert.h"
#include "Common/clientInformation.h"

#include <memory> // for std::unique_ptr

struct ClientContext {

    serverOptions options;

    std::shared_ptr<ITransport> transport;
    std::shared_ptr<IMessageConvert> messageConvert;
    std::shared_ptr<ICommandConvert> commandConvert;

    std::shared_ptr<IState> state;

    clientInformation clientInfo;
};

#endif // CLIENTCONTEXT_H