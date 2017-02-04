#ifndef INCLUDED_CORE_WORM_HEAD_COMPONENT_H
#define INCLUDED_CORE_WORM_HEAD_COMPONENT_H

#include "i_worm_head_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>

class WormHeadComponent : public IWormHeadComponent
{
public:
    WormHeadComponent();
    virtual void SetPrevPositions( PrevPositions_t const& prevPositions );
    virtual IWormHeadComponent::PrevPositions_t& GetPrevPositions();
    virtual void SetLength( int32_t length );
    virtual int32_t GetLength() const;
    virtual void SetLengthDecrease( int32_t lengthDecrease );
    virtual int32_t GetLengthDecrease() const;
    virtual void SetBodyId( int32_t bodyId );
    virtual int32_t GetBodyId() const;
    virtual void SetTailId( int32_t tailId );
    virtual int32_t GetTailId() const;
    virtual void SetSpawnDelay( double spawnDelay );
    virtual double GetSpawnDelay() const;
    virtual void SetSpawnDelayMax( double spawnDelayMax );
    virtual double GetSpawnDelayMax() const;
    virtual void SetSpawnCount( int32_t spawnCount );
    virtual int32_t GetSpawnCount() const;
    virtual void SetFirstBuild( bool firstBuild );
    virtual bool IsFirstBuild() const;
    virtual void SetBodyParts( BodyParts_t const& bodyParts );
    virtual IWormHeadComponent::BodyParts_t& GetBodyParts();
    virtual void SetWaitDistance( double waitDistance );
    virtual double GetWaitDistance() const;
    virtual void SetGapPercent( double gapPercent );
    virtual double GetGapPercent() const;
    virtual void CopyFrom( IWormHeadComponent* iHeadC );
protected:
    friend class ComponentFactory;
    int32_t mLength;
    int32_t mLengthDecrease;
    int32_t mBodyId;
    int32_t mTailId;
    double mSpawnDelay;
    double mSpawnDelayMax;
    int32_t mSpawnCount;
    bool mFirstBuild;
    BodyParts_t mBodyParts;
    PrevPositions_t mPrevPositions;
    double mWaitDistance;
    double mGapPercent;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void WormHeadComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IWormHeadComponent>(*this);
    ar& mLength;
    ar& mLengthDecrease;
    ar& mBodyId;
    ar& mTailId;
    ar& mSpawnDelay;
    ar& mSpawnDelayMax;
    ar& mSpawnCount;
    ar& mFirstBuild;
    ar& mBodyParts;
    ar& mPrevPositions;
    ar& mWaitDistance;
    ar& mGapPercent;
}

class WormHeadComponentLoader : public ComponentLoader<WormHeadComponent>
{
    virtual void BindValues();
protected:
    WormHeadComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( WormHeadComponent, WormHeadComponent, "worm_head_component" );

#endif//INCLUDED_CORE_WORM_HEAD_COMPONENT_H



//command:  "classgenerator.exe" -g "component" -c "worm_head_component" -m "int32_t-length int32_t-lengthDecrease int32_t-bodyId int32_t-tailId double-spawnDelay double-spawnDelayMax int32_t-spawnCount bool-firstBuild BodyParts_t-bodyParts"
