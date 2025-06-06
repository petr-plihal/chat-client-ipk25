#ifndef JOINSTATE_H
#define JOINSTATE_H

#include "IState.h"

class JoinState : public IState {
public:
    explicit JoinState(ClientContext& cont)
        : IState(cont) { // Initialize the base class IState
    }

    void HandleCommand(const Command& command) override;

    void HandleMessage(const Message& message) override;
};

#endif // JOINSTATE_H