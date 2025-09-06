#ifndef _Socket_H_
#define _Socket_H_

#include "func.h"
#include "Noncopyable.h"

namespace byk
{
class Socket
    :public Noncopyable
{
    public:
        Socket();
        explicit
            Socket(int fd)
            :_fd(fd)
            {}
        ~Socket();
        void shutdownWrite();
        int fd()const{return _fd;};
    private:
        int _fd;
};
}
#endif

