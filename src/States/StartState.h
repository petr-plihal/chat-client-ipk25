#ifndef STARTSTATE_H
#define STARTSTATE_H

#include "IState.h"

class StartState : public IState {
public:
    explicit StartState(ClientContext& cont)
        : IState(cont) { // Initialize the base class IState
    }

    void HandleCommand(const Command& command) override;

    void HandleMessage(const Message& message) override;
};

#endif // STARTSTATE_H