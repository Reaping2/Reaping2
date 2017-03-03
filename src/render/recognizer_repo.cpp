#include "platform/i_platform.h"
#include "render/recognizer_repo.h"
#include "platform/filesystem_utils.h"
namespace render {

RecognizerRepo::RecognizerRepo()
    : mRecognizerFactory( RecognizerFactory::Get() )
{
    Init();
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

    fs_utils::for_each( "actors", ".render", [&]( Json::Value const& desc )
    {
        std::string actorNameStr;
        if (!Json::GetStr( desc["name"], actorNameStr ))
        {
            return;
        }
        int32_t actorId = AutoId( actorNameStr );
        Recognizers_t& recognizers = mRecognizersMap[actorId];

        auto const& recognizersJValue = desc["recognizers"];
        if (!recognizersJValue.isArray()|| recognizersJValue.empty())
        {
            return;
        }
        int32_t orderindex = 0;
        for (auto&& recognizerJValue : recognizersJValue)
        {
            std::string recognizerName;
            if (!Json::GetStr( recognizerJValue["recognizer"], recognizerName ))
            {
                continue;
            }
            std::auto_ptr<Recognizer> recognizer( mRecognizerFactory( AutoId( recognizerName ) ) );
            if (recognizer.get() == nullptr)
            {
                continue;
            }

            std::string rendererName;
            if (!Json::GetStr( recognizerJValue["renderer"], rendererName ))
            {
                continue;
            }
            recognizer->SetActionRenderer( AutoId( rendererName ) );
            recognizer->SetOrder( orderindex++ );
            recognizers.push_back( recognizer.release() );
        }
    } );


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
