#ifndef __NOCOPYABLE_H__
#define __NOCOPYABLE_H__
    



namespace dog
{
class Nocopyalbe 
{
protected:
    Nocopyalbe() {}
    ~Nocopyalbe() {}
    
    Nocopyalbe(const Nocopyalbe &) = delete;
    Nocopyalbe & operator=(const Nocopyalbe &) = delete;
    
};

class Copeyable
{

};

}
#endif
