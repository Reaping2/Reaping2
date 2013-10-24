#ifndef INCLUDED_PLATFORM_LOG_H
#define INCLUDED_PLATFORM_LOG_H

class Logger : public Singleton<Logger>
{
	friend class Singleton<Logger>;
	Logger(){}
public:
	void Log(char const* format, ...);
};

#define LOG(...) Logger::Get().Log(__VA_ARGS__)
//#define LOG(...) (void)(__VA_ARGS__)

#endif//INCLUDED_PLATFORM_LOG_H
