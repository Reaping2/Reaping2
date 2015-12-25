#ifndef INCLUDED_CORE_I_EXPLODE_H
#define INCLUDED_CORE_I_EXPLODE_H

class IExplode
{
public:
    virtual void SetExplosionProjectile(int32_t explosionProjectile)=0;
    virtual int32_t GetExplosionProjectile()const=0;
    virtual void SetCount(int32_t count)=0;
    virtual int32_t GetCount()const=0;
    virtual void SetScatter(double scatter)=0;
    virtual double GetScatter()const=0;
};

#endif//INCLUDED_CORE_I_EXPLODE_H
