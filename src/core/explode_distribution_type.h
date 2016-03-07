#ifndef INCLUDED__EXPLODE_DISTRIBUTION_TYPE_H
#define INCLUDED__EXPLODE_DISTRIBUTION_TYPE_H

#include "platform/singleton.h"
#include "boost/bimap.hpp"

class ExplodeDistributionType : public platform::Singleton<ExplodeDistributionType>
{
protected:
    friend class platform::Singleton<ExplodeDistributionType>;
    ExplodeDistributionType();
public:
    enum Type
    {
        Normal = 0,
        Random,
        Num_Classes
    };
    ExplodeDistributionType::Type operator()( int32_t Id ) const;
    int32_t operator()( ExplodeDistributionType::Type type ) const;
private:
    typedef boost::bimap<int32_t, ExplodeDistributionType::Type> IdToExplodeDistributionTypeMap_t;
    IdToExplodeDistributionTypeMap_t mIdToExplodeDistributionTypeMap;
};

#endif//INCLUDED__EXPLODE_DISTRIBUTION_TYPE_H


//command:  "classgenerator.exe" -g "enum" -c "explode_distribution_type" -m "Normal-normal Random-random"
