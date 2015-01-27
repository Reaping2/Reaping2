#ifndef INCLUDED_RENDER_RECOGNIZER_FACTORY_H
#define INCLUDED_RENDER_RECOGNIZER_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "recognizer.h"
namespace render {
class RecognizerFactory : public platform::Factory<Recognizer>, public platform::Singleton<RecognizerFactory>
{
    friend class platform::Singleton<RecognizerFactory>;
    RecognizerFactory();
};
} // namespace render


#endif//INCLUDED_RENDER_RECOGNIZER_FACTORY_H