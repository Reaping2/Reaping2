#include "platform/i_platform.h"
#include "buff_holder_system.h"
#include "core/buffs/i_buff_holder_component.h"

namespace engine {

BuffHolderSystem::BuffHolderSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
{
}


void BuffHolderSystem::Init()
{
    SubSystemHolder::Init();
}


void BuffHolderSystem::Update( double DeltaTime )
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
        if ( !buffHolderC.IsValid() )
        {
            continue;
        }

        Opt<Buff> prevBuff;
        //         for( BuffList_t::iterator buffIt = buffList.begin(), buffE = buffList.end(); buffIt != buffE; ++buffIt )
        //         {
        //             Buff& buff = **buffIt;
        //             if (prevBuff.IsValid()&&prevBuff->GetType()==buff.GetType())
        //             {
        //                 continue;
        //             }
        //
        //             prevBuff=*buffIt;
        //             BindIds_t::iterator buffssIt=mSubSystems.get<SubSystemHolder::AllByBindId>().find(buff.GetType());
        //             if (buffssIt!=mSubSystems.get<SubSystemHolder::AllByBindId>().end())
        //             {
        //                 buffssIt->mSystem->Update(actor,DeltaTime);
        //             }
        //         }
        for( SubSystems_t::iterator ssIt = mSubSystems.begin(), e = mSubSystems.end(); ssIt != e; ++ssIt )
        {
            ssIt->mSystem->Update( actor, DeltaTime );
        }

        BuffList_t& buffList = buffHolderC->GetBuffList();
        std::list<Opt<Buff> > newBuffList;
        for( BuffList_t::iterator buffIt = buffList.begin(), buffE = buffList.end(); buffIt != buffE; ++buffIt )
        {
            Buff& buff = **buffIt;
            double currSecsToEnd = buff.GetSecsToEnd();
            double newSecsToEnd = currSecsToEnd - DeltaTime;
            if ( newSecsToEnd > 0 )
            {
                buff.SetSecsToEnd(newSecsToEnd); //updates all secs to end so order wont be broken.
                newBuffList.push_back(*buffIt);
            }
            else
            {
                delete buffIt->Get();
            }
        }
        buffList.clear();
        buffList.insert(newBuffList.begin(), newBuffList.end());
    }
}


} // namespace engine

