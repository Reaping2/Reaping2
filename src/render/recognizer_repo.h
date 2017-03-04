#ifndef INCLUDED_RENDER_RECOGNIZER_REPO_H
#define INCLUDED_RENDER_RECOGNIZER_REPO_H
#include "platform/i_platform.h"

#include "platform/singleton.h"
#include "recognizer_factory.h"
namespace render {
class RecognizerRepo : public platform::Singleton<RecognizerRepo>
{
public:
    typedef boost::ptr_list<Recognizer> Recognizers_t;
    Recognizers_t const* GetRecognizers( int32_t actorId ) const;

    typedef std::set<int32_t> ExcludedRecognizers_t;
    ExcludedRecognizers_t const* GetExcludedRecognizers( int32_t recognizerId ) const;
private:
    friend class platform::Singleton<RecognizerRepo>;
    RecognizerRepo();

    typedef std::map<int32_t, Recognizers_t> RecognizersMap_t;
    RecognizersMap_t mRecognizersMap;
    RecognizerFactory& mRecognizerFactory;

    typedef std::map<int32_t, ExcludedRecognizers_t> ExcludedRecognizersMap_t;
    ExcludedRecognizersMap_t mExcludedRecognizersMap;
    void Init();
    bool AddRecognizerExcludesFromOneDesc( Json::Value& RecognizerExcludesDesc );
};
} // namespace render


#endif//INCLUDED_RENDER_RECOGNIZER_REPO_H
