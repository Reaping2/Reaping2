#ifndef INCLUDED_CORE_I_WORM_HEAD_COMPONENT_H
#define INCLUDED_CORE_I_WORM_HEAD_COMPONENT_H

#include "component.h"
#include <vector>
#include "platform/i_platform.h"
#include "platform/serialize_vec4.h"

class IWormHeadComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IWormHeadComponent)
    struct PrevPosition
    {
        PrevPosition( glm::vec2 position, double orientation, double distance )
            : mPosition( position )
            , mOrientation( orientation )
            , mDistance( distance )
        {}
        PrevPosition() = default;
        glm::vec2 mPosition;
        double mOrientation = 0.0;
        double mDistance = 0.0;

        friend class ::boost::serialization::access;
        template<class Archive>
        void serialize( Archive& ar, const unsigned int version );
    };
    typedef std::list<PrevPosition> PrevPositions_t;
    virtual void SetPrevPositions( PrevPositions_t const& prevPositions ) = 0;
    virtual PrevPositions_t& GetPrevPositions() = 0;
    virtual void SetLength(int32_t length)=0;
    virtual int32_t GetLength()const=0;
    virtual void SetLengthDecrease(int32_t lengthDecrease)=0;
    virtual int32_t GetLengthDecrease()const=0;
    virtual void SetBodyId(int32_t bodyId)=0;
    virtual int32_t GetBodyId()const=0;
    virtual void SetTailId(int32_t tailId)=0;
    virtual int32_t GetTailId()const=0;
    virtual void SetSpawnDelay(double spawnDelay)=0;
    virtual double GetSpawnDelay()const=0;
    virtual void SetSpawnDelayMax(double spawnDelayMax)=0;
    virtual double GetSpawnDelayMax()const=0;
    virtual void SetSpawnCount(int32_t spawnCount)=0;
    virtual int32_t GetSpawnCount()const=0;
    virtual void SetFirstBuild(bool firstBuild)=0;
    virtual bool IsFirstBuild()const=0;
    typedef std::vector<int32_t> BodyParts_t;
    virtual void SetBodyParts(BodyParts_t const& bodyParts)=0;
    virtual BodyParts_t& GetBodyParts()=0;
    virtual void SetWaitDistance( double waitDistance ) = 0;
    virtual double GetWaitDistance()const = 0;
    virtual void SetGapPercent( double gapPercent ) = 0;
    virtual double GetGapPercent()const = 0;
    virtual void CopyFrom(IWormHeadComponent* iHeadC) = 0;
public:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IWormHeadComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

template<class Archive>
void IWormHeadComponent::PrevPosition::serialize( Archive& ar, const unsigned int version )
{
    ar&mPosition;
    ar&mOrientation;
    ar&mDistance;
}

#endif//INCLUDED_CORE_I_WORM_HEAD_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "worm_head_component" -m "int32_t-length int32_t-lengthDecrease int32_t-bodyId int32_t-tailId double-spawnDelay double-spawnDelayMax int32_t-spawnCount bool-firstBuild BodyParts_t-bodyParts"
