#ifndef IMESSAGECONVERT_H
#define IMESSAGECONVERT_H

#include "Common/message.h"

#include "Output/debug.h"
#include "Output/Output.h"

#include <string>
#include <vector>

class IMessageConvert {
public:
    virtual ~IMessageConvert() = default;

    virtual std::string EncodeMessage(const Message& message) = 0;

    virtual Message DecodeMessage(const std::string& data) = 0;

    virtual std::vector<Message> DecodeStream(const std::string& data) = 0; // UDP will just always return vector of 1 message
};

#endif // IMESSAGECONVERT_H