#include "SocketIO.h"
#include "func.h"
namespace byk{
int SocketIO::recvPeek(char* buff,int len) const
{
    int ret=-1;
    do{
        ret=recv(_fd,buff,len,MSG_PEEK);
    }while(ret==-1&&errno==EINTR);
    return ret;
}

int SocketIO::recvn(char* buff,int len)
{
    int left=len;
    char* pbuf=buff;
    while(left>0)
    {
        int ret=recv(_fd,pbuf,left,0);
        if(ret==-1&&errno==EINTR)
        {continue;}
        else if(ret==-1)
        {
            perror("recv");
            return len-left;
        }
        else if(ret==0)
        {
            return len-left;
        }
        else{
            left-=ret;
            pbuf+=ret;
        }
    }
    return len-left;
}

int SocketIO::readline(char* buff,int maxlen)
{
    char* pbuf=buff;
    int left=maxlen-1;
    int total=0;
    while(left>0)
    {
        int ret=recvPeek(pbuf,left);
        for(int i=0;i<ret;++i)
        {
            if(pbuf[i]=='\n')
            {
                int sz=i+1;
                ret=recvn(pbuf,sz);
                total+=ret;
                pbuf[i]='\0';
                return total;
            }
        }
        ret=recvn(pbuf,ret);
        left-=ret;
        pbuf+=ret;
        total+=ret;
    }
    buff[maxlen-1]='\0';
    return maxlen-1;
}

int SocketIO::sendn(const char* buff,int len)
{
    int left=len;
    const char* pbuf=buff;
    while(left>0)
    {
        int ret=send(_fd,pbuf,left,0);
        if(ret==-1&&errno==EINTR)
        {
            continue;
        }
        else if(ret==-1)
        {
            perror("send");
            return len-left;
        }
        else
        {
            left-=ret;
            pbuf+=ret;
        }
    }
    return len-left;
}

}
