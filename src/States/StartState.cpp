#include "StartState.h"

// States this state can transition to
#include "AuthState.h"

void StartState::HandleCommand(const Command& command) {
    printf_debug("Entered [StartState], handling command");

    Message outgoingMessage;
    
    switch (command.type) {

    case CommandType::AUTH:
        printf_debug("Handling AUTH command");

        outgoingMessage.type = MessageType::AUTH;
        outgoingMessage.username = command.username;
        outgoingMessage.secret = command.secret;
        outgoingMessage.displayName = command.displayName;

        /* It is okay to set the information immediately, since in case of bad information, !REPLY will be received.
           In that case, the user will be prompted again for the information. */
        context.clientInfo.username = command.username;
        context.clientInfo.secret = command.secret;
        context.clientInfo.displayName = command.displayName;

        context.transport->SendData(
            context.messageConvert->EncodeMessage(outgoingMessage)
        );

        context.state = std::make_shared<AuthState>(context);

        break;

    case CommandType::JOIN:
        printf_debug("Handling JOIN command");
        Error("Cannot join channel without authentication. See /help for more information.");
        break;

    case CommandType::RENAME:
        printf_debug("Handling RENAME command");
        // TODO: check if the name is valid
        context.clientInfo.displayName = command.displayName;
        break;
    
    case CommandType::HELP:
        printf_debug("Handling HELP command");
        HelpForCommands();
        break;

    case CommandType::MESSAGE:
        printf_debug("Handling MESSAGE command");
        Error("Cannot send message without authentication. See /help for more information.");
        break;

    case CommandType::INVALID:
        printf_debug("Handling INVALID command");
        Error("Unrecognized command, check syntax. See /help for more information.");
        break;

    default:
        printf_debug("Handling UNDEFINED command");
        throw std::runtime_error("Internal error: command type not initialized to a valid value.");
        break;
    }

}

void StartState::HandleMessage(const Message& message) {
    printf_debug("Entered [StartState], handling message");

    Message outgoingMessage;

    switch (message.type) {

    case MessageType::ERR:
        printf_debug("Handling ERR message");
        Info("ERROR FROM " + message.displayName + ": " + message.messageContent);
        context.state = nullptr;
        break;

    case MessageType::BYE:
        printf_debug("Handling BYE message");
        Error("Server closed connection. Exiting...");
        context.state = nullptr;
        break;

    case MessageType::UNDEFINED:
        printf_debug("Handling UNDEFINED message");
        Error("Internal error: message type not initialized to a valid value.");
        outgoingMessage.type = MessageType::BYE;
        outgoingMessage.displayName = context.clientInfo.displayName;
        outgoingMessage.messageContent = "Internal error: message type not initialized to a valid value.";
        context.transport->SendData(
            context.messageConvert->EncodeMessage(outgoingMessage)
        );
        context.state = nullptr;
        break;

    default:
        printf_debug("Handling message of type not supported in this state");
        Error("Unexpected message from server. Exiting...");
        outgoingMessage.type = MessageType::ERR;
        outgoingMessage.displayName = context.clientInfo.displayName;
        outgoingMessage.messageContent = "Received message of type not supported in current state.";
        context.transport->SendData(
            context.messageConvert->EncodeMessage(outgoingMessage)
        );
        context.state = nullptr;
        break;
    }
}