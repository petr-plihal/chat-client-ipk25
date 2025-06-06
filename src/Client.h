#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <memory> // TODO: remove? For std::unique_ptr - to automatically manage memory of the state object(s)

#include "Common/clientContext.h"
#include "States/IState.h"

class Client {
public:

    Client(ClientContext& cont)
        : context(cont) {
        // Initialize the state machine
        }

    void Run();

// The order of declaration is important, because:
//    - it dictates the order of destruction
//    - it has to correspond to the order of initialization in the constructor !
private:
    ClientContext& context;
};

#endif // CLIENT_H