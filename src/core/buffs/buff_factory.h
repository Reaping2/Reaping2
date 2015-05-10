#ifndef INCLUDED_CORE_BUFF_FACTORY_H
#define INCLUDED_CORE_BUFF_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "buff.h"

namespace core {

class BuffFactory : public platform::Factory<Buff>, public platform::Singleton<BuffFactory>
{
    friend class platform::Singleton<BuffFactory>;
    BuffFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<Buff> CreateBuff( int32_t Id );
};

template<typename Elem_T>
std::auto_ptr<Buff> BuffFactory::CreateBuff( int32_t Id )
{
    return std::auto_ptr<Buff>( new Elem_T() );
}

} // namespace core

#endif//INCLUDED_CORE_BUFF_FACTORY_H
