#include "i_texcombiner.h"

namespace fs=boost::filesystem;
TexCombiner Combiner;

void BuildFilesList(fs::path Dir)
{
	Combiner.SetDestinationSize(1024,1024);
	if(!fs::exists(Dir)||!fs::is_directory(Dir))
		return;
	for(fs::recursive_directory_iterator dir_iter(Dir),end_iter;dir_iter!=end_iter;++dir_iter)
	{
		if(!fs::is_regular_file(dir_iter->status()))
			continue;
		const fs::path& p=*dir_iter;
		Combiner.AddTexture(p);
	}
	Combiner.Save("combined");
}

int main()
{
	BuildFilesList("texcombiner");
	return 0;
}
