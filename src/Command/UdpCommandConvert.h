#ifndef UDPCOMMANDCONVERT_H
#define UDPCOMMANDCONVERT_H

#include "ICommandConvert.h"

class UdpCommandConvert : public ICommandConvert {
public:
    std::string EncodeCommand(const Command& command) const override;
    Command DecodeCommand(const std::string& data) const override;
};

#endif // UDPCOMMANDCONVERT_H