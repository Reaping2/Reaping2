#ifndef INCLUDED_CORE_RENDERABLE_LAYER_H
#define INCLUDED_CORE_RENDERABLE_LAYER_H
#include "platform/singleton.h"
#include <map>
#include "json/json.h"
#include "platform/i_platform.h"

class RenderableLayer : public platform::Singleton<RenderableLayer>
{
protected:
    friend class platform::Singleton<RenderableLayer>;
    RenderableLayer();
public:
    int32_t operator()( std::string const& Name ) const;
    typedef std::map<std::string, int32_t> NameToPriority_t;
    NameToPriority_t const& GetNameToPriorityMap() const;
private:
    NameToPriority_t mNameToPriority;
    void Init();
    void AddLayerFromOneDesc( Json::Value const& Desc );
    int32_t mNextPrio = 0;
};


#endif//INCLUDED_CORE_RENDERABLE_LAYER_H
