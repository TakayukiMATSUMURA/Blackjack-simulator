#pragma once

#include "./uncopyable.h"

template <class HostClass>
class Singleton : private UnCopyable<Singleton<HostClass>>
{
public:
    static HostClass *instance()
    {
        if (_instance == nullptr)
            _instance = HostClass::create();

        return _instance;
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

private:
    static HostClass *_instance;
};

template <class HostClass>
HostClass *Singleton<HostClass>::_instance = nullptr;
