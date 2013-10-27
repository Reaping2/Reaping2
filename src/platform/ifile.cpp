#include "i_platform.h"

bool File::ReadAll( std::string& Data )
{
	if(!IsValid())return false;
	return Read(Data,GetSize()-GetPosition());
}
