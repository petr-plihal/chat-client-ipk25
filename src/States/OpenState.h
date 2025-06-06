#ifndef OPENSTATE_H
#define OPENSTATE_H

#include "IState.h"

class OpenState : public IState {
public:
    explicit OpenState(ClientContext& cont)
        : IState(cont) { // Initialize the base class IState
    }

    void HandleCommand(const Command& command) override;

    void HandleMessage(const Message& message) override;
};

#endif // OPENSTATE_H