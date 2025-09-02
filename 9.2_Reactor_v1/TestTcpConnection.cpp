#include "Acceptor.h"
#include "TcpConnection.h"
#include <iostream>
using std::cout;
using std::endl;


int main()
{
    byk::Acceptor acceptor(8000);
    acceptor.ready();
    printf(">> server start listening.\n");
    int netfd=acceptor.accept();
    byk::TcpConnection conn(netfd);
    cout<<conn.toString()<<" has connected successfully."<<endl;
    conn.send("welcome to server.");
    cout<<"receive msg: "<<conn.receive()<<endl;
    
    return 0;
}

