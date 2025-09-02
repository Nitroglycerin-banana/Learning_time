#ifndef _Acceptor_H_
#define _Acceptor_H_

#include "InetAddress.h"
#include "Socket.h"
#include <sys/stat.h>
namespace byk{
class Acceptor{
public:
    Acceptor(unsigned short port,const string& ip="127.0.0.1");
    void ready();
    int accept();
    int fd() const{return _listensock.fd();}
private:
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void bind();
    void listen();
private:
    InetAddress _addr;
    Socket _listensock;
};
}
#endif

