#ifndef _Task_H_
#define _Task_H_
#include <functional>

namespace wgy{
class Task{
public:
    using CallBack=std::function<void()>;
};
}
#endif

