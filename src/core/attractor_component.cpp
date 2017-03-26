#include "core/attractor_component.h"

AttractorComponent::AttractorComponent()
    : mAttractorDescs()
{
}

void AttractorComponent::SetAttractorDescs( AttractorDescs_t attractorDescs )
{
    mAttractorDescs = attractorDescs;
}

IAttractorComponent::AttractorDescs_t& AttractorComponent::GetAttractorDescs()
{
    return mAttractorDescs;
}



void AttractorComponentLoader::BindValues()
{
    auto const& json = (*mSetters)["attractor_descs"];
    if (json.isArray())
    {
        IAttractorComponent::AttractorDescs_t descs;
        for (auto&& part : json)
        {
            IAttractorComponent::AttractorDesc desc;
            desc.Load( part );
            descs.push_back( desc );
            Bind<IAttractorComponent::AttractorDescs_t>( &AttractorComponent::SetAttractorDescs, descs );
        }
    }
}

AttractorComponentLoader::AttractorComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( AttractorComponent, AttractorComponent );
