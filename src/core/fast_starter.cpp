#include "fast_starter.h"
#include "platform/settings.h"
#include "ui/i_ui.h"

namespace core {


FastStarter::FastStarter( std::string const& shortcut )
    : mMainModel( "thread", &RootModel::Get() )
    , mSleepModel( IntFunc( this, &FastStarter::Sleep ), "sleep", &mMainModel )
{
    std::vector<std::string> shortcutSplit;
    boost::split( shortcutSplit, shortcut, boost::is_any_of( "/|" ) );
    auto const name = shortcutSplit.front();
    auto jsonArr = Settings::Get().Resolve("fast_start."+ name );
    if (jsonArr.isArray())
    {
        for (auto&& Desc : jsonArr)
        {
            for (auto&& actions : Desc["action"])
            {
                for (auto&& action : actions)
                {
                    if (action.isString())
                    {
                        auto const actStr = action.asString();
                        auto const paramStr = "@param";
                        if (boost::starts_with( actStr, paramStr ))
                        {
                            auto const woParamStr = actStr.substr( std::strlen( paramStr ) );
                            int paramInd = atoi( woParamStr.substr( 0, woParamStr.find( "|" ) ).c_str() );
                            if (paramInd < shortcutSplit.size())
                            {
                                action = Json::Value( shortcutSplit[paramInd] );
                            }
                            else
                            {
                                auto findInd = actStr.find( "|" ) ;
                                if (findInd != std::string::npos)
                                {
                                    L1( "default val: %s \n", actStr.substr( findInd + 1 ).c_str() );
                                    action = Json::Value( actStr.substr( findInd + 1 ) );
                                }
                            }
                        }
                    }
                }
            }
            Button button( AutoId( "button" ) );
            button.Init( Desc );
            button.Trigger();
        }
    }
}

void FastStarter::Sleep( int32_t millis )
{
    boost::this_thread::sleep( boost::posix_time::milliseconds( millis ) );
}

} // namespace core
