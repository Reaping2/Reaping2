#ifndef INCLUDED_CORE_GATLING_GUN_H
#define INCLUDED_CORE_GATLING_GUN_H
#include "core/weapon.h"
#include "platform/export.h"

class GatlingGun : public Weapon
{
public:
    GatlingGun( int32_t Id );
    GatlingGun();
    enum DeployState
    {
        Undeployed = 0,
        Deploying,
        Deployed,
        Undeploying
    };
    virtual bool CanSwitch() const;
    virtual bool IsShooting() const;
    virtual bool IsShootingAlt() const;
    void SetWindup( double windup );
    double GetWindup()const;
    void SetWindupMax( double windupMax );
    double GetWindupMax()const;
    void SetDeploy( double deploy );
    double GetDeploy()const;
    void SetDeployMax( double deployMax );
    double GetDeployMax()const;
    void SetDeployState( DeployState deployState );
    DeployState GetDeployState()const;

    virtual bool CanReload() const;
    virtual glm::vec3 GetMouseColor() const;
    virtual double GetMouseSize() const;
    virtual std::string GetMouseText() const;
    virtual bool IsMouseResizable() const;
protected:
    friend class ComponentFactory;
    double mWindup;
    double mWindupMax;

    double mDeploy;
    double mDeployMax;
    DeployState mDeployState;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void GatlingGun::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Weapon>( *this );
    ar& mWindup;
    ar& mWindupMax;
    ar& mDeploy;
    ar& mDeployMax;
    ar& mDeployState;
}

class GatlingGunLoader: public ItemLoader<GatlingGun>
{
public:
    virtual void BindValues();
    GatlingGunLoader();
    friend class ItemLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( GatlingGun, GatlingGun, "gatling_gun" );
#endif//INCLUDED_CORE_GATLING_GUN_H
