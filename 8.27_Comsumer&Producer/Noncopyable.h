#ifndef _Noncopyable_H_
#define _Noncopyable_H_

namespace byk{
class Noncopyable
{
protected:
    Noncopyable(){}
    ~Noncopyable(){}
    Noncopyable(const Noncopyable&)=delete;
    Noncopyable& operator=(const Noncopyable&)=delete;
};
}
#endif

