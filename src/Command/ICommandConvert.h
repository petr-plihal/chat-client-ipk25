#ifndef ICOMMANDCONVERT_H
#define ICOMMANDCONVERT_H

#include "Common/command.h"

#include "Output/debug.h"

#include <string>

class ICommandConvert {
public:
    virtual ~ICommandConvert() = default;

    virtual std::string EncodeCommand(const Command& command) const = 0;

    virtual Command DecodeCommand(const std::string& data) const = 0;
};

#endif // ICOMMANDCONVERT_H