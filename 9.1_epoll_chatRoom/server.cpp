#include "func.h"
#include <cstdlib>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <set>

#define EVENT_ARR_SIZE 100
int main()
{
    //创建服务器的监听套接字
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }
    printf(">> listenfd is %d.\n",listenfd);

    //设置套接字的属性，网络地址可以重用
    int on=1;
    int ret=setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(ret<0)
    {
        perror("setsockopt");
        close(listenfd);
        return EXIT_FAILURE;
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8000);
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");//代表本地环回地
                                                      //址

    //绑定网络地址
    ret=bind(listenfd,(const struct sockaddr*)&serveraddr,sizeof(serveraddr));
    if(ret<0)
    {
        perror("bind");
        close(listenfd);
        return EXIT_FAILURE;
    }

    //服务器进行监听
    ret=listen(listenfd,20000);//C10K大并发问题 
    printf(">> 服务器正在监听.\n");

    //创建epoll实例
    int epfd=epoll_create1(0);
    if(epfd<0)
    {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }
    printf(">> 创建的epoll套接字epfd= %d.\n",epfd);
    //监听epoll上的读事件
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd=listenfd;
    ev.events=EPOLLIN;
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    if(ret<0)
    {
        perror("epoll_ctl");
        return EXIT_FAILURE;
    }
    //准备在用户态中存储已经就绪好的文件描述符及其事件的存储数组
    struct epoll_event evtArr[EVENT_ARR_SIZE]={};
    std::map<int,int> conns;//存储已经建立好的连接以及上一次交互时间
    //事件循环
    while(1)
    {
        printf("epoll等待就绪事件发生中.\n");
        int nready=epoll_wait(epfd,evtArr,EVENT_ARR_SIZE,1000);
        printf("当前发生的就绪事件数:%d\n",nready);
        if(nready==-1&&errno==EINTR)
        {
            continue;
        }
        else if(nready==0)
        {
            //只有1s以内没有任何文件描述符就绪时，才会执行超时分支
            printf("\n1s超时.\n");
        }
        else if(nready==-1)
        {
            perror("epoll_wait");
            return EXIT_FAILURE;
        }
        else
        {
            //nready>0的情况，说明有nready个文件描述符就绪了
            //对nready个就绪的文件描述符进行处理
            for(int i=0;i<nready;++i)
            {
                int fd=evtArr[i].data.fd;
                if(fd==listenfd)
                {
                    //对新连接进行处理
                    struct sockaddr_in clientaddr;
                    memset(&clientaddr,0,sizeof(clientaddr));
                    socklen_t len=sizeof(clientaddr);
                    printf("\n现在在接收新的socket之前.\n");
                    //获取与对端进行交互的netfd(分机)
                    int netfd=accept(listenfd,(struct sockaddr*)&clientaddr,&len);
                    if(netfd<0)
                    {
                        perror("accept");
                        continue;
                    }
                    //打印客户端的网络地址
                    printf("\n客户端[套接字号：%d/%s:%d] 已经连接.\n",netfd,inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
                    //由epoll对netfd的读事件进行监听
                    ev.data.fd=netfd;
                    ev.events=EPOLLIN|EPOLLET;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,&ev);
                    //添加到conns中
                    conns[netfd]=time(nullptr);
                }
                else
                {
                    //已经建立好的连接中对netfd的处理
                    char buff[1000]={0};
                    //从内核接收缓冲区拷贝数据
                    printf(">>>读事件触发.\n");
                    ret=recv(fd,buff,sizeof(buff),0);
                    if(ret==0)
                    {
                        printf("\n %d 套接字已经关闭.\n",fd);
                        ev.data.fd=fd;
                        //从epoll的监听红黑树上删除fd
                        epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&ev);
                        conns.erase(fd);//在conns中删除掉
                        close(fd);//关闭连接
                        continue;
                    }
                    conns[fd]=time(nullptr);//更新上一次交互的时间
                    printf("\n>> 接收到 %d 字节,信息:%s\n",ret,buff);//打印接受的数据到终端上
                    //进行业务逻辑的处理（转发给聊天室的所有其他人）
                    for(auto & elem:conns)
                    {
                        if(elem.first!=fd)//除了发送者不转发
                        {
                            ret=send(elem.first,buff,strlen(buff),0);
                        }
                    }
                    printf("\n>>发送 %d 字节.\n",ret);
                 }
             }
        }
            //在这里踢人
            std::set<int> deleteFds;
            int curTime=time(nullptr);
            for(auto& elem:conns)
            {
                if(curTime-elem.second>=10)
                {
                    close(elem.first);
                    printf("\n conn中的 %d 超时了,被踢掉.\n",elem.first);
                    //在遍历中不适合做删除操作
                    //很有可能造成迭代器失效的问题
                    deleteFds.insert(elem.first);//先记录下要删除的元素
                }
            }
            for(auto& fd:deleteFds)
            {
                conns.erase(fd);
            }
        }
    return 0;
}

