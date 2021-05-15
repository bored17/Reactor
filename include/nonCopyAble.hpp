#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__
 

namespace dog
{
    class NonCopyAble 
    {
    protected:
        NonCopyAble() {}
        ~NonCopyAble() {}
        NonCopyAble(const NonCopyAble &) = delete;
        NonCopyAble & operator=(const NonCopyAble &) = delete;
    };
}
#endif
