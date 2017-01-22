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
    std::string typeStr;
    if (!Json::GetStr( RecognizerDesc["type"], typeStr ) || typeStr != "recognizer_desc") // mandatory
    {
        return true;
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
            continue;
        }
        std::auto_ptr<Recognizer> recognizer( mRecognizerFactory( AutoId( recognizerName ) ) );
        if( recognizer.get() == nullptr )
        {
            continue;
        }

        std::string rendererName;
        if ( !Json::GetStr( recognizerJValue["renderer"], rendererName ) )
        {
            continue;
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
    Filesys& FSys( Filesys::Get() );

    PathVect_t Paths;
    FSys.GetFileNames( Paths, "actors" );
    for (auto const& Path : Paths)
    {
        if (Path.extension().string() != ".json")
        {
            continue;
        }
        AutoFile JsonFile = FSys.Open( Path );
        if (!JsonFile.get())
        {
            continue;
        }
        JsonReader Reader( *JsonFile );
        if (!Reader.IsValid())
        {
            continue;
        }
        Json::Value Root = Reader.GetRoot();
        if (!Root.isArray())
        {
            continue;
        }
        for (auto& Desc : Root)
        {
            try
            {
                if (!AddRecognizerFromOneDesc( Desc ))
                {
                    return;
                }
            }
            catch (std::exception const& err)
            {
                L1( "Exception caught while parsing %s : %s", Path.generic_string().c_str(), err.what() );
            }
        }
    }


    AutoFile F = FSys.Open( boost::filesystem::path( "misc/recognizers_excludes.json" ) );
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
    for( auto& RecognizerExcludeDesc : Root)
    {
        if( !AddRecognizerExcludesFromOneDesc( RecognizerExcludeDesc ) )
        {
            return;
        }
    }
}

RecognizerRepo::Recognizers_t const* RecognizerRepo::GetRecognizers( int32_t actorId ) const
{
    auto it = mRecognizersMap.find( actorId );
    return it == mRecognizersMap.end() ? nullptr : &it->second;
}

RecognizerRepo::ExcludedRecognizers_t const* RecognizerRepo::GetExcludedRecognizers( int32_t recognizerId ) const
{
    auto it = mExcludedRecognizersMap.find( recognizerId );
    return it == mExcludedRecognizersMap.end() ? nullptr : &it->second;
}

} // namespace render
