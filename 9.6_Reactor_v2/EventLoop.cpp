#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "func.h"

namespace byk
{
EventLoop::EventLoop(Acceptor& ac)
    :_epfd(createEpollFd())
     ,_acceptor(ac)
     ,_isLooping(false)
     ,_evtArr(1024)
    {
        addEpollReadEvent(_acceptor.fd());
    }

EventLoop::~EventLoop()
{
    if(_epfd>0)
    {
        close(_epfd);
    }
}

void EventLoop::loop()
{
    _isLooping=true;
    while(_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::waitEpollFd()
{
    int nready=epoll_wait(_epfd,_evtArr.data(),_evtArr.size(),5000);
    if(nready==-1&&errno==EINTR)
    {
        return;
    }
    else if(nready==-1)
    {
        perror("epoll_wait");
        return;
    }
    else if(nready==0)
    {
        printf("epoll timeout.\n");
        return;
    }
    else
    {
        for(int i=0;i<nready;++i)
        {
            int fd=_evtArr[i].data.fd;
            if(fd==_acceptor.fd())
            {
                handleNewConnection();
            }
            else
            {
                handleMessage(fd);
            }
        }
    }
}

int EventLoop::createEpollFd()
{
    int fd=epoll_create1(0);
    if(fd<0)
    {
        perror("epoll_create1");
    }
    return fd;
}

void EventLoop::addEpollReadEvent(int fd)
{
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.events=EPOLLIN;
    ev.data.fd=fd;
    int ret=epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
    if(ret<0)
    {
        perror("epoll_ctl");
    }
}

void EventLoop::delEpollReadEvent(int fd)
{
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd=fd;
    int ret=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
    if(ret<0)
    {
        perror("epoll_ctl");
    }
}

void EventLoop::handleNewConnection()
{
    int netfd=_acceptor.accept();
    if(netfd<0)
    {
        return;
    }
    addEpollReadEvent(netfd);
    TcpConnectionPtr conn(new TcpConnection(netfd));
    conn->setAllCallbacks(_onConnection,_onMessage,_onClose);
    conn->handleNewConnectionCallback();
    _conns[netfd]=conn;
}

void EventLoop::handleMessage(int fd)
{
    auto iter=_conns.find(fd);
    if(iter!=_conns.end())
    {
        bool isClosed=iter->second->isClosed();
        if(isClosed)
        {
            iter->second->handleCloseCallback();
            delEpollReadEvent(fd);
            _conns.erase(fd);
        }
        else
        {
            iter->second->handleMessageCallback();
        }
    }
}
}
