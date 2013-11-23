#include "i_platform.h"
#include <cstdio>
#include <cstdarg>

void Logger::Log(int Level,char const* format, ... )
{
	if(mDisabledLevels&(1<<Level))
		return;
	va_list arg;
	int done;

	va_start (arg, format);
	done = vfprintf (stderr, format, arg);
	va_end (arg);
}

Logger::Logger()
: mDisabledLevels(0)
{
	AutoFile f(new OsFile("debug.json"));
	if(!f.get()||!f->IsValid())return;
	JsonReader Reader(*f.get());
	if(!Reader.IsValid())return;
	Json::Value& Root=Reader.GetRoot();
	if(!Root.isObject())return;
	Json::Value& DisabledLogs=Root["disabled_logs"];
	if(!DisabledLogs.isArray())return;
	const size_t NumDisabledLogs=DisabledLogs.size();
	for(size_t i=0;i<NumDisabledLogs;++i)
	{
		Json::Value& DisLog=DisabledLogs[i];
		uint32_t Val;
		if(!GetUInt(DisLog,Val))continue;
		mDisabledLevels|=(1<<Val);
	}
}

