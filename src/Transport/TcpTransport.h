#ifndef TCPTRANSPORT_H
#define TCPTRANSPORT_H

#include "ITransport.h"
#include <string>

class TcpTransport : public ITransport {
public:
    TcpTransport() = default;
    ~TcpTransport();

    void EstablishConnection(const serverOptions& options) override;
    void SendData(const std::string& data) override;
    std::string ReceiveData() override;
    void CloseSocket() override;
    int GetSocket() const override { return socketDescriptor; }
};

#endif // TCPTRANSPORT_H