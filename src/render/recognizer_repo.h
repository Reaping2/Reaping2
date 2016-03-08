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
    bool HasRecognizers( int32_t actorId );
    Recognizers_t& GetRecognizers( int32_t actorId );

    typedef std::set<int32_t> ExcludedRecognizers_t;
    bool HasExcludedRecognizers( int32_t recognizerId );
    ExcludedRecognizers_t& GetExcludedRecognizers( int32_t recognizerId );
private:
    friend class platform::Singleton<RecognizerRepo>;
    RecognizerRepo();

    typedef std::map<int32_t, Recognizers_t> RecognizersMap_t;
    RecognizersMap_t mRecognizersMap;
    RecognizerFactory& mRecognizerFactory;

    typedef std::map<int32_t, ExcludedRecognizers_t> ExcludedRecognizersMap_t;
    ExcludedRecognizersMap_t mExcludedRecognizersMap;
    void Init();
    bool AddRecognizerFromOneDesc( Json::Value& RecognizerDesc );
    bool AddRecognizerExcludesFromOneDesc( Json::Value& RecognizerExcludesDesc );
};
} // namespace render


#endif//INCLUDED_RENDER_RECOGNIZER_REPO_H