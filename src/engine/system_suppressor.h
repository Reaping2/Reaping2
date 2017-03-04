#ifndef INCLUDED_ENGINE_SYSTEM_SUPPRESSOR_H
#define INCLUDED_ENGINE_SYSTEM_SUPPRESSOR_H

#include <map>
#include <set>
#include "core/map_load_event.h"
#include "core/map_start_event.h"
#include "platform/i_platform.h"

namespace engine {

class SystemSuppressor : public platform::Singleton<SystemSuppressor>
{
protected:
    friend class platform::Singleton<SystemSuppressor>;
    SystemSuppressor();
public:
    enum Type : int32_t
    {
        SceneLoad=0,
        Force,
        None
    };
    void Add( SystemSuppressor::Type type, int32_t systemType );
    bool Suppress( SystemSuppressor::Type type, bool takeOwnership = false );
    bool Resume( SystemSuppressor::Type type );

    bool IsSuppressed() const;
    Type GetOwnerSystemType() const;
private:
    void ResumeSystemStates();

    typedef std::set<int32_t> Systems_t;
    typedef std::map<SystemSuppressor::Type, Systems_t> TypeToSystems_t;
    TypeToSystems_t mTypeToSystems;

    Type mOwnerSystemType = None;
    typedef std::map<int32_t, bool> SystemStates_t;
    SystemStates_t mSystemStates;
};


struct SuppressEvent : public platform::Event
{
    SystemSuppressor::Type mState;
    bool mSuppressed;
    SuppressEvent( SystemSuppressor::Type state, bool suppressed )
        : mState( state ), mSuppressed( suppressed ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_SYSTEM_SUPPRESSOR_H


//command:  "classgenerator.exe" -g "enum" -n "engine" -c "system_suppressor" -m "SceneLoad-MapLoad"
