#ifndef ITRANSPORT_H
#define ITRANSPORT_H

#include <string>

#include "Common/serverOptions.h"

#include "Output/debug.h"

class ITransport {
public:
    virtual ~ITransport() = default;

    virtual void EstablishConnection(const serverOptions& options) = 0;
    virtual void SendData(const std::string& data) = 0;
    virtual std::string ReceiveData() = 0;
    virtual void CloseSocket() = 0;
    virtual int GetSocket() const = 0;
protected:
    int socketDescriptor = -1;
};

#endif // ITRANSPORT_H