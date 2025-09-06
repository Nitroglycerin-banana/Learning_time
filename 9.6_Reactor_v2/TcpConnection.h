#ifndef _TcpConnection_H_
#define _TcpConnection_H_

#include "Noncopyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <string>
#include <memory>
#include <functional>
using std::string;
using std::shared_ptr;
using std::function;
namespace byk
{
class TcpConnection;
using TcpConnectionPtr=shared_ptr<TcpConnection>;
using TcpConnectionCallback=function<void(TcpConnectionPtr)>;
class TcpConnection
: public Noncopyable,public std::enable_shared_from_this<TcpConnection>
{
public:
    explicit
    TcpConnection(int fd);
    ~TcpConnection();
    void setAllCallbacks(const TcpConnectionCallback& cb1,
                         const TcpConnectionCallback& cb2,
                         const TcpConnectionCallback& cb3)//这里不能使用右值引用，否则只有第一次连接有效，第二次获取回调函数只能得到空指针
    {
        _onConnection=cb1;
        _onMessage=cb2;
        _onClose=cb3;
    }
    bool isClosed() const;
    string receive();
    void send(const string& msg);
    string toString() const;

    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
};
    
}
#endif

