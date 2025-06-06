#ifndef AUTHSTATE_H
#define AUTHSTATE_H

#include "IState.h"

class AuthState : public IState {
public:
    explicit AuthState(ClientContext& cont)
        : IState(cont) { // Initialize the base class IState
    }

    void HandleCommand(const Command& command) override;

    void HandleMessage(const Message& message) override;
};

#endif // AUTHSTATE_H