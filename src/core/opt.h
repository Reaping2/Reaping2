#ifndef INCLUDED_CORE_OPT_H
#define INCLUDED_CORE_OPT_H

template<typename T>
class Opt
{
public:
    Opt(T* Ptr=NULL);
    bool IsValid()const;
    T* operator->() const;
    T& operator*() const;
    T* operator->();
    T& operator*();
    T* Get() const;
    T* Get();
private:
    mutable T* mPtr;
};

template<typename T>
T* Opt<T>::Get()
{
    return mPtr;
}

template<typename T>
T* Opt<T>::Get() const
{
    return mPtr;
}

template<typename T>
Opt<T>::Opt(T* Ptr)
{
    mPtr=Ptr;
}

template<typename T>
bool Opt<T>::IsValid()const
{
    return mPtr!=NULL;
}

template<typename T>
T* Opt<T>::operator->() const
{
    return mPtr;
}

template<typename T>
T& Opt<T>::operator*() const
{
    return *mPtr;
}

template<typename T>
T* Opt<T>::operator->()
{
    return const_cast<T*>(((const Opt*)this)->operator->());
}

template<typename T>
T& Opt<T>::operator*()
{
    return const_cast<T&>(((const Opt*)this)->operator*());
}
#endif//INCLUDED_CORE_OPT_H