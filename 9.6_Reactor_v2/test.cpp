#include "TcpConnection.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include <iostream>

using std::cout;
using std::endl;
void onConnection(byk::TcpConnectionPtr conn)
{
    cout<<conn->toString()<<" has connected successfully "<<endl;
}

void onMessage(byk::TcpConnectionPtr conn)
{
    string msg=conn->receive();
    cout<<"recv msg:"<<msg<<endl;
    conn->send(msg);
}

void onClose(byk::TcpConnectionPtr conn)
{
   cout<< conn->toString()<<" has closed "<<endl;
}

int main()
{
    byk::Acceptor acceptor(8000);
    acceptor.ready();
    printf("<< server start listening.\n");
    byk::EventLoop loop(acceptor);
    loop.setAllCallbacks(onConnection,onMessage,onClose);
    loop.loop();
    return 0;
}
