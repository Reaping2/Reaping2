#include "core/buffs/buff_holder_component.h"

BuffHolderComponent::BuffHolderComponent()
{
}

BuffList_t& BuffHolderComponent::GetBuffList()
{
    return mBuffList.mAllBuffs;
}

void BuffHolderComponent::AddBuff(std::auto_ptr<Buff> buff)
{
    mBuffList.mAllBuffs.insert(Opt<Buff>(buff.release()));
}

BuffHolderComponent::~BuffHolderComponent()
{
    for( BuffList_t::iterator it = mBuffList.mAllBuffs.begin(), e = mBuffList.mAllBuffs.end(); it!=e; ++it )
    {
        delete (*it).Get();
    }
    mBuffList.mAllBuffs.clear();
}



void BuffHolderComponentLoader::BindValues()
{
}

BuffHolderComponentLoader::BuffHolderComponentLoader()
{
}
