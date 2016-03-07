#include "platform/i_platform.h"
#include "render/recognizer_repo.h"
namespace render {

RecognizerRepo::RecognizerRepo()
    : mRecognizerFactory( RecognizerFactory::Get() )
{
    Init();
}

bool RecognizerRepo::AddRecognizerFromOneDesc( Json::Value& RecognizerDesc )
{

    std::string actorNameStr;
    if( !Json::GetStr( RecognizerDesc["name"], actorNameStr ) )
    {
        return false;
    }
    int32_t actorId = AutoId( actorNameStr );
    Recognizers_t& recognizers = mRecognizersMap[actorId];

    Json::Value& recognizersJValue = RecognizerDesc["recognizers"];
    if ( !recognizersJValue.isArray() )
    {
        return false;
    }
    if ( recognizersJValue.empty() )
    {
        return true;
    }
    int32_t orderindex = 0;
    for( Json::Value::iterator i = recognizersJValue.begin(), e = recognizersJValue.end(); i != e; ++i )
    {
        Json::Value& recognizerJValue = *i;
        std::string recognizerName;
        if ( !Json::GetStr( recognizerJValue["recognizer"], recognizerName ) )
        {
            return false;
        }
        std::auto_ptr<Recognizer> recognizer( mRecognizerFactory( AutoId( recognizerName ) ) );

        std::string rendererName;
        if ( !Json::GetStr( recognizerJValue["renderer"], rendererName ) )
        {
            return false;
        }
        recognizer->SetActionRenderer( AutoId( rendererName ) );
        recognizer->SetOrder( orderindex++ );
        recognizers.push_back( recognizer.release() );
    }
    return true;
}

bool RecognizerRepo::AddRecognizerExcludesFromOneDesc( Json::Value& RecognizerExcludesDesc )
{

    std::string excludedRecognizerNameStr;
    if( !Json::GetStr( RecognizerExcludesDesc["name"], excludedRecognizerNameStr ) )
    {
        return false;
    }
    int32_t excludedRecognizerId = AutoId( excludedRecognizerNameStr );
    ExcludedRecognizers_t& excludedRecognizers = mExcludedRecognizersMap[excludedRecognizerId];

    Json::Value& recognizerExcludesJValue = RecognizerExcludesDesc["excludes"];
    if ( !recognizerExcludesJValue.isArray() )
    {
        return false;
    }
    if ( recognizerExcludesJValue.empty() )
    {
        return true;
    }
    for( Json::Value::iterator i = recognizerExcludesJValue.begin(), e = recognizerExcludesJValue.end(); i != e; ++i )
    {
        Json::Value& recognizerJValue = *i;
        std::string excludeName;
        if ( !Json::GetStr( recognizerJValue, excludeName ) )
        {
            return false;
        }
        excludedRecognizers.insert( AutoId( excludeName ) );
    }
    return true;
}

void RecognizerRepo::Init()
{
    Filesys& Fsys( Filesys::Get() );
    AutoFile F = Fsys.Open( boost::filesystem::path( "recognizers.json" ) );
    if( !F.get() || !F->IsValid() )
    {
        return;
    }
    JsonReader Reader( *F );
    if( !Reader.IsValid() )
    {
        return;
    }
    Json::Value Root = Reader.GetRoot();
    if( !Root.isArray() )
    {
        return;
    }
    for( Json::Value::iterator i = Root.begin(), e = Root.end(); i != e; ++i )
    {
        Json::Value& RecognizerDesc = *i;
        if( !AddRecognizerFromOneDesc( RecognizerDesc ) )
        {
            return;
        }
    }

    F = Fsys.Open( boost::filesystem::path( "recognizers_excludes.json" ) );
    if( !F.get() || !F->IsValid() )
    {
        return;
    }
    JsonReader Reader2( *F );
    if( !Reader2.IsValid() )
    {
        return;
    }
    Root = Reader2.GetRoot();
    if( !Root.isArray() )
    {
        return;
    }
    for( Json::Value::iterator i = Root.begin(), e = Root.end(); i != e; ++i )
    {
        Json::Value& RecognizerExcludeDesc = *i;
        if( !AddRecognizerExcludesFromOneDesc( RecognizerExcludeDesc ) )
        {
            return;
        }
    }
}

RecognizerRepo::Recognizers_t& RecognizerRepo::GetRecognizers( int32_t actorId )
{
    return mRecognizersMap.find( actorId )->second;
}

bool RecognizerRepo::HasRecognizers( int32_t actorId )
{
    return mRecognizersMap.find( actorId ) != mRecognizersMap.end();
}

bool RecognizerRepo::HasExcludedRecognizers( int32_t recognizerId )
{
    return mExcludedRecognizersMap.find( recognizerId ) != mExcludedRecognizersMap.end();
}

RecognizerRepo::ExcludedRecognizers_t& RecognizerRepo::GetExcludedRecognizers( int32_t recognizerId )
{
    return mExcludedRecognizersMap.find( recognizerId )->second;
}

} // namespace render