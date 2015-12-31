#ifndef INCLUDED_CORE_GATLING_GUN_H
#define INCLUDED_CORE_GATLING_GUN_H
#include "core/weapon.h"

class GatlingGun : public Weapon
{
    GatlingGun( int32_t Id );
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
};

#endif//INCLUDED_CORE_GATLING_GUN_H
