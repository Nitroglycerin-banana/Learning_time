#include "TcpConnection.h"
#include <sstream>

namespace byk
{
TcpConnection::TcpConnection(int fd)
    :_sock(fd)
     ,_sockIO(fd)
     ,_localAddr(getLocalAddr())
     ,_peerAddr(getPeerAddr())
    {}

bool TcpConnection::isClosed() const
{
    char buff[20]={0};
    int ret=_sockIO.recvPeek(buff,sizeof(buff));
    return ret=0;
}

string TcpConnection::receive()
{
    char buff[65535]={0};
    int ret=_sockIO.readline(buff,sizeof(buff));
    return string(buff,ret);
}
void TcpConnection::send(const string& msg)
{
    int ret=_sockIO.sendn(msg.data(),msg.size());
    printf(">> send %d bytes.\n",ret);
}


string TcpConnection::toString() const
{
    std::ostringstream oss;
    oss<<"tcp "<<_localAddr.ip()<<":"<<_localAddr.port()
        <<" -> "<<_peerAddr.ip()<<":"<<_peerAddr.port();
    return oss.str();
}

InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    memset(&addr,0,sizeof(addr));
    int ret=getsockname(_sock.fd(),(struct sockaddr*)&addr,&len);
    if(ret<0)
    {
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    memset(&addr,0,sizeof(addr));
    int ret=getpeername(_sock.fd(),(struct sockaddr*)&addr,&len);
    if(ret<0)
    {
        perror("getpeername");
    }
    return InetAddress(addr);
}
}
