#ifndef _TcpConnection_H_
#define _TcpConnection_H_

#include "Noncopyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <string>
using std::string;

namespace byk{
class TcpConnection:Noncopyable
{
public:
    explicit
        TcpConnection(int fd);
    bool isClosed() const;
    string receive();
    void send(const string& msg);
    string toString() const;
private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
};
}
#endif

