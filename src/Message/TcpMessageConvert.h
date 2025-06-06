#ifndef TCPMESSAGECONVERT_H
#define TCPMESSAGECONVERT_H

#include "IMessageConvert.h"

class TcpMessageConvert : public IMessageConvert {
public:
    std::string EncodeMessage(const Message& message) override;
    Message DecodeMessage(const std::string& data) override;
    std::vector<Message> DecodeStream(const std::string& data) override;
    std::string incompleteMessage;
};

#endif // TCPMESSAGECONVERT_H