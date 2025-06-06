#include "AuthState.h"

// States this state can transition to
#include "OpenState.h"

void AuthState::HandleCommand(const Command& command) {
    printf_debug("Entered [AuthState], handling command");
    
    Message outgoingMessage;

    switch (command.type) {

    case CommandType::AUTH:
        printf_debug("Handling AUTH command");

        outgoingMessage.type = MessageType::AUTH;
        outgoingMessage.username = command.username;
        outgoingMessage.secret = command.secret;
        outgoingMessage.displayName = command.displayName;

        context.clientInfo.username = command.username;
        context.clientInfo.secret = command.secret;
        context.clientInfo.displayName = command.displayName;
        
        context.transport->SendData(
            context.messageConvert->EncodeMessage(outgoingMessage)
        );

        break;

    case CommandType::JOIN:
        printf_debug("Handling JOIN command");
        Error("You are not authenticated. Please authenticate first. See /help for more information.");
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
        Error("You are not authenticated. Please authenticate first. See /help for more information.");
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

void AuthState::HandleMessage(const Message& message) {
    printf_debug("Entered [AuthState], handling message");

    Message outgoingMessage;

    switch (message.type) {
    case MessageType::REPLY:
        printf_debug("Handling REPLY message");
        if (message.replySuccess) {
            Info("Action Success: " + message.messageContent);
            context.state = std::make_shared<OpenState>(context);
        } else {
            Info("Action Failure: " + message.messageContent);
        }
        break;

    case MessageType::MSG:
        printf_debug("Handling MSG message");
        Error("Server response issue. Exiting...");

        outgoingMessage.type = MessageType::ERR;
        outgoingMessage.displayName = context.clientInfo.displayName;
        outgoingMessage.messageContent = "Did not expect a message in AUTH state.";

        context.transport->SendData(
            context.messageConvert->EncodeMessage(outgoingMessage)
        );

        context.state = nullptr;
        break;
    
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