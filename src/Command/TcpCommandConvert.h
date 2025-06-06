#ifndef TCPCOMMANDCONVERT_H
#define TCPCOMMANDCONVERT_H

#include "ICommandConvert.h"

class TcpCommandConvert : public ICommandConvert {
public:
    std::string EncodeCommand(const Command& command) const override;
    Command DecodeCommand(const std::string& data) const override;
};

#endif // TCPCOMMANDCONVERT_H