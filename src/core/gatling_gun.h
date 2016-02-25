#ifndef INCLUDED_CORE_GATLING_GUN_H
#define INCLUDED_CORE_GATLING_GUN_H
#include "core/weapon.h"
#include <boost/serialization/export.hpp>

class GatlingGun : public Weapon
{
    GatlingGun( int32_t Id );
    GatlingGun();

    void InitMembers();

    friend class Factory<Item>;
public:
    enum DeployState
    {
        Undeployed=0,
        Deploying,
        Deployed,
        Undeploying
    };

    virtual bool IsShooting() const;
    virtual bool IsShootingAlt() const;
    void SetWindup(double windup);
    double GetWindup()const;
    void SetWindupMax(double windupMax);
    double GetWindupMax()const;
    void SetWindupSpeed(double windupSpeed);
    double GetWindupSpeed()const;
    void SetDeploy(double deploy);
    double GetDeploy()const;
    void SetDeployMax(double deployMax);
    double GetDeployMax()const;
    void SetDeploySpeed(double deploySpeed);
    double GetDeploySpeed()const;
    void SetDeployState(DeployState deployState);
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
    double mWindupSpeed;

    double mDeploy;
    double mDeployMax;
    double mDeploySpeed;
    DeployState mDeployState;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void GatlingGun::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
    ar & mWindup;
    ar & mWindupMax;
    ar & mWindupSpeed;
    ar & mDeploy;
    ar & mDeployMax;
    ar & mDeploySpeed;
    ar & mDeployState;
}


BOOST_CLASS_EXPORT_KEY2(GatlingGun,"gatling_gun");
#endif//INCLUDED_CORE_GATLING_GUN_H
