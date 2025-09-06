#include "TcpConnection.h"
#include <sstream>
#include <iostream>

namespace byk
{
TcpConnection::TcpConnection(int fd)
    :_sock(fd)
     ,_sockIO(fd)
     ,_localAddr(getLocalAddr())
     ,_peerAddr(getPeerAddr())
    {}


TcpConnection::~TcpConnection()
{
    std::cout<<"~TcpConnection()"<<std::endl;
}
void TcpConnection::handleNewConnectionCallback()
{
    if(_onConnection)
    {
        _onConnection(shared_from_this());//这里如果传shared_ptr<TcpConnection>(this)指针会造成智能指针的误用，
                                          //即同一个资源被不同的智能指针所托管，
                                          //在析构的时候会产生double free的问题
                                          //因为在EventLoop中的map里会存指向TcpConnection的智能指针
                                          //如果这里用this指针给回调函数传参
                                          //那this指针就会被两个智能指针托管
                                          //导致double free问题
    }
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessage)
    {
        _onMessage(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback()
{
    if(_onClose)
    {
        _onClose(shared_from_this());
    }
}

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
    oss<<"tcp"<<_localAddr.ip()<<":"<<_localAddr.port()
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
