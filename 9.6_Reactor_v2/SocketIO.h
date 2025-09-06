#ifndef _SocketIO_H_
#define _SocketIO_H_

namespace byk
{
class SocketIO{
public:
    SocketIO(int fd)
        :_fd(fd)
    {}
    int recvPeek(char* buff,int len)const;
    int recvn(char* buff,int len);
    int readline(char* buff,int maxlen);
    int sendn(const char* buff,int len);
private:
    int _fd;

};
}
#endif

