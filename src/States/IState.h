#ifndef ISTATE_H
#define ISTATE_H

#include "Common/message.h"
#include "Common/command.h"
#include "Common/clientContext.h"

#include "Transport/ITransport.h"
#include "Message/IMessageConvert.h"

#include "Output/debug.h"
#include "Output/Output.h"

#include <string>

class IState {
public:
    explicit IState(ClientContext& cont)
        : context(cont) {
    }
    virtual ~IState() = default;

    virtual void HandleCommand(const Command& command) = 0;
    virtual void HandleMessage(const Message& message) = 0;

protected:
    ClientContext& context;

};

#endif // ISTATE_H