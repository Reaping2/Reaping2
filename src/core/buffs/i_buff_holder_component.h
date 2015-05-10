#ifndef INCLUDED_CORE_I_BUFF_HOLDER_COMPONENT_H
#define INCLUDED_CORE_I_BUFF_HOLDER_COMPONENT_H

#include "core/component.h"
#include "platform/i_platform.h"
#include "buff.h"

using namespace ::boost::multi_index;
class BuffHolder
{
public:

    struct BuffOrderer
    { 
        typedef int32_t result_type;
        result_type operator()(const Opt<Buff>& buff)const
        {
            return buff->GetType();
        }
    };
    struct SecsToEndOrderer
    { 
        typedef double result_type;
        result_type operator()(const Opt<Buff>& buff)const
        {
            return buff->GetSecsToEnd();
        }
    };

    typedef multi_index_container<
        Opt<Buff>,
        indexed_by<
            ordered_non_unique<
                composite_key<
                    Opt<Buff>,
                    BuffHolder::BuffOrderer
                >
            >,
            ordered_non_unique<
                composite_key<
                    Opt<Buff>,
                    BuffHolder::SecsToEndOrderer
                >
            >
        >
    > BuffList_t;
    typedef BuffList_t::nth_index<1>::type BuffListSecsToEnd_t;
    BuffList_t mAllBuffs;

};

typedef BuffHolder::BuffList_t BuffList_t;

template<int N>
class BuffListFilter
{
public:
    typedef BuffList_t::const_iterator const_iterator;
protected:
    BuffList_t::const_iterator mI;
    BuffList_t::const_iterator mE;
    size_t mSize;
public:
    BuffListFilter(BuffList_t const& bufflist)
    {
        mI=bufflist.begin();
        mE=bufflist.end();
        mSize=bufflist.size();
    }
    const_iterator begin()
    {
        return mI;
    }
    const_iterator end()
    {
        return mE;
    }
    size_t size()
    {
        return mSize;
    }
};

template<>
class BuffListFilter<0>
{
public:
    typedef BuffList_t::nth_index<0>::type::const_iterator const_iterator;
protected:
    const_iterator mI;
    const_iterator mE;
    size_t mSize;
public:
    BuffListFilter(BuffList_t const& bufflist, int32_t buffStaticType)
    {
        boost::tie(mI,mE)=bufflist.get<0>().equal_range(boost::make_tuple(buffStaticType));
        mSize=std::distance(mI,mE);
    }
    const_iterator begin()
    {
        return mI;
    }
    const_iterator end()
    {
        return mE;
    }
    size_t size()
    {
        return mSize;
    }
};

template<>
class BuffListFilter<1>
{
public:
    typedef BuffList_t::nth_index<1>::type::const_iterator const_iterator;
protected:
    const_iterator mI;
    const_iterator mE;
    size_t mSize;
public:
    BuffListFilter(BuffList_t const& bufflist, int32_t secsToEnd)
    {
        boost::tie(mI,mE)=bufflist.get<1>().equal_range(boost::make_tuple(secsToEnd));
        mSize=std::distance(mI,mE);
    }
    const_iterator begin()
    {
        return mI;
    }
    const_iterator end()
    {
        return mE;
    }
    size_t size()
    {
        return mSize;
    }
};

class IBuffHolderComponent : public Component
{
public:
    enum BuffIndex
    {
        All=0,
        SecsToEnd=1
    };
    virtual BuffList_t& GetBuffList()=0;
    virtual void AddBuff(std::auto_ptr<Buff> buff)=0;
    DEFINE_COMPONENT_BASE(IBuffHolderComponent)
};

#endif//INCLUDED_CORE_I_BUFF_HOLDER_COMPONENT_H
