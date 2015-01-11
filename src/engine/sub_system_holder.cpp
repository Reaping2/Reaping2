#include "platform/i_platform.h"
#include "engine/sub_system_holder.h"
#include "platform/auto_id.h"

using platform::AutoId;
namespace engine {
    SubSystemElement::SubSystemElement(int32_t id, int32_t bindedId, int32_t order, bool enabled, Opt<SubSystem> system)
        : mId(id)
        , mBindedId(bindedId)
        , mOrder(order)
        , mEnabled(enabled)
        , mSystem(system)
    {

    }


    int32_t SubSystemHolder::SubSystemDefaultOrderer::operator()(const SubSystemElement& system) const
    {
        return system.mOrder;
    }


    int32_t SubSystemHolder::SubSystemIdOrderer::operator()(const SubSystemElement& system) const
    {
        return system.mId;
    }


    int32_t SubSystemHolder::SubSystemBindIdOrderer::operator()(const SubSystemElement& system) const
    {
        return system.mBindedId;
    }


    bool SubSystemHolder::IsEnabled::operator()(const SubSystemElement& system) const
    {
        return system.mEnabled;
    }


    void SubSystemHolder::AddSubSystem(int32_t BindId, int32_t Id)
    {
        std::auto_ptr<SubSystem> sys = mSubSystemFactory(Id);
        int32_t typ = sys->GetType();
        mSubSystems.insert(SubSystemElement(typ,BindId,mSubSystems.size(),true,Opt<SubSystem>(sys.release())));
    }

    Opt<SubSystem> SubSystemHolder::GetSubSystem(int32_t BindId) const
    {
        BindIds_t const& BindIdSubSystens=mSubSystems.get<SubSystemHolder::AllByBindId>();
        BindIds_t::const_iterator it=
            BindIdSubSystens.find(BindId);
        return Opt<SubSystem>(const_cast<SubSystem*>(
            it == BindIdSubSystens.end()?NULL:it->mSystem.Get())); 
    }

    Opt<SubSystem> SubSystemHolder::GetSubSystem(int32_t BindId)
    {
        return ( ( const SubSystemHolder* )this )->GetSubSystem(BindId);
    }

    void SubSystemHolder::SetEnabled(int32_t BindId, bool enabled)
    {
        BindIds_t& BindIdSubSystens=mSubSystems.get<SubSystemHolder::AllByBindId>();
        BindIds_t::iterator it=
            BindIdSubSystens.find(BindId);
        if (it!=BindIdSubSystens.end())
        {
            BindIdSubSystens.modify(it,SubSystemEnableModifier(enabled));
        }
    }

    SubSystemHolder::SubSystemHolder()
        : mSubSystemFactory(SubSystemFactory::Get())
    {
    }

    SubSystemHolder::~SubSystemHolder()
    {
        for( SubSystems_t::iterator it = mSubSystems.begin(), e = mSubSystems.end(); it != e; ++it )
        {
            delete it->mSystem.Get();
        }
        mSubSystems.clear();
    }

    void SubSystemHolder::Init()
    {
        for( SubSystems_t::iterator it = mSubSystems.begin(), e = mSubSystems.end(); it != e; ++it )
        {
            it->mSystem->Init();
        }
    }


    SubSystemEnableModifier::SubSystemEnableModifier(bool enabled)
        : mEnabled(enabled)
    {

    }

    void SubSystemEnableModifier::operator()(SubSystemElement& system)
    {
        system.mEnabled=mEnabled;
    }

} // namespace engine
