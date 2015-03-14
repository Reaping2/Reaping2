#include "platform/i_platform.h"
#include <boost/program_options.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>
#include <string>
#include <ctype.h>

class Generator
{
protected:
    std::string classCamelCase;
    std::string classUpperCaseUnderscore;
    std::string parentCamelCase;
    std::string nameSpaceUpperCase;
    std::string headerGuard;
public:
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase)
    {
        L1("default generator implementation, do nothing\n");
    }
    void Init(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase)
    {
        classCamelCase=UnderScoreToCamelCase(classUnderscore);
        parentCamelCase=UnderScoreToCamelCase(classUnderscore);
        classUpperCaseUnderscore=boost::to_upper_copy(classUnderscore);
        nameSpaceUpperCase=boost::to_upper_copy(namespaceLowerCase);
        headerGuard="INCLUDED_"+nameSpaceUpperCase+"_"+classUpperCaseUnderscore+"_H";
    }
    virtual ~Generator() {}
    std::string UnderScoreToCamelCase(std::string underscore)
    {
        boost::to_lower(underscore);
        std::vector<std::string> parts;
        boost::split(parts,underscore,boost::is_any_of("_"));
        for(std::vector<std::string>::iterator i=parts.begin(),e=parts.end();i!=e;++i)
        {
            (*i)[0]=toupper((*i)[0]);
        }
        std::string camelCase;
        camelCase=boost::join(parts,camelCase);
        return camelCase;
    }
};

class IComponentGenerator : public Generator
{
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase)
    {
        L1("IComponentGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="component";
        }
        Init(classUnderscore,parentUnderscore,namespaceLowerCase);

        AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
        fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
        fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
        fprintf(file.mFile, "\n");
        fprintf(file.mFile, "#include \"component.h\"\n");
        fprintf(file.mFile, "\n");
        fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
        fprintf(file.mFile, "{\n");
        fprintf(file.mFile, "public:\n");
        fprintf(file.mFile, "    DEFINE_COMPONENT_BASE(%s)\n",classCamelCase.c_str());
        fprintf(file.mFile, "};\n");
        fprintf(file.mFile, "\n");
        fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());
        L1("IComponentGenerator ended\n");
    }
};

class ComponentGenerator : public Generator
{
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase)
    {
        L1("ComponentGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="i_"+classUnderscore;
        }
        Init(classUnderscore,parentUnderscore,namespaceLowerCase);

        AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
        fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
        fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
        fprintf(file.mFile, "\n");
        fprintf(file.mFile, "#include \"%s.h\"\n",parentUnderscore.c_str());
        fprintf(file.mFile, "#include \"core/property_loader.h\"\n");
        fprintf(file.mFile, "\n");
        fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
        fprintf(file.mFile, "{\n");
        fprintf(file.mFile, "public:\n");
        fprintf(file.mFile, "protected:\n");
        fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
        fprintf(file.mFile, "    friend class ComponentFactory;\n");
        fprintf(file.mFile, "private:\n");
        fprintf(file.mFile, "};\n");
        fprintf(file.mFile, "\n");

        fprintf(file.mFile, "class %sLoader : public ComponentLoader<%s>\n",classCamelCase.c_str(),classCamelCase.c_str());
        fprintf(file.mFile, "{\n");
        fprintf(file.mFile, "    virtual void BindValues();\n");
        fprintf(file.mFile, "protected:\n");
        fprintf(file.mFile, "    %sLoader();\n",classCamelCase.c_str());
        fprintf(file.mFile, "    friend class ComponentLoaderFactory;\n");
        fprintf(file.mFile, "};\n");
        fprintf(file.mFile, "\n");

        fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

        fprintf(file.mFile, "\\TODO: to component_factory.cpp:\n");
        fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateComponent<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
        fprintf(file.mFile, "\\TODO: to component_loader_factory.cpp:\n");
        fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateComponentLoader<%sLoader>);\n",classUnderscore.c_str(),classCamelCase.c_str());

        
        AutoNormalFile filecpp((classUnderscore+".cpp").c_str(),"w" );
        fprintf(filecpp.mFile, "#include \"core/%s.h\"\n",classUnderscore.c_str());
        fprintf(filecpp.mFile, "\n");
        fprintf(filecpp.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
        fprintf(filecpp.mFile, "{\n");
        fprintf(filecpp.mFile, "}\n");
        fprintf(filecpp.mFile, "\n");
        fprintf(filecpp.mFile, "\n");
        fprintf(filecpp.mFile, "%sLoader::BindValues()\n",classCamelCase.c_str());
        fprintf(filecpp.mFile, "{\n");
        fprintf(filecpp.mFile, "}\n");
        fprintf(filecpp.mFile, "\n");
        fprintf(filecpp.mFile, "%sLoader::%sLoader()\n",classCamelCase.c_str(),classCamelCase.c_str());
        fprintf(filecpp.mFile, "{\n");
        fprintf(filecpp.mFile, "}\n");


        L1("ComponentGenerator ended\n");
    }
};

class GeneratorFactory : public platform::Factory<Generator>, public platform::Singleton<GeneratorFactory>
{
    friend class platform::Singleton<GeneratorFactory>;
    template<typename Elem_T>
    static std::auto_ptr<Generator> CreateGenerator( int32_t Id );

    GeneratorFactory()
    {
        Bind( AutoId( "default_generator" ), &CreateGenerator<Generator> );
        SetDefault( AutoId( "default_generator" ) );
        Bind( AutoId( "i_component" ), &CreateGenerator<IComponentGenerator> );
        Bind( AutoId( "component" ), &CreateGenerator<ComponentGenerator> );
    }
};

template<typename Elem_T>
std::auto_ptr<Generator> GeneratorFactory::CreateGenerator( int32_t Id )
{
    std::auto_ptr<Generator> generator( new Elem_T() );
    return generator;
}

int main(int argc, char* argv[])
{
	namespace po=boost::program_options;
	// Declare the supported options.
	po::options_description desc("Allowed options");
	std::string classUnderscore;
	std::string generatorType;
    std::string parentUnderscore;
    std::string namespaceLowerCase;
	desc.add_options()
        ("help", "produce help message")
		("-c", po::value<std::string>(&classUnderscore), "class_name_underscore")
		("-g", po::value<std::string>(&generatorType), "type of generator")
		("-p", po::value<std::string>(&parentUnderscore), "parent_name_underscore")
        ("-n", po::value<std::string>(&namespaceLowerCase), "namespacelowercase")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }
    if (!vm.count("-g")) {
        L1("need to specify a valid generator!");
        return 2;
    }

    GeneratorFactory::Get()(AutoId(generatorType))->Generate(classUnderscore,parentUnderscore,namespaceLowerCase);

	return 0;
}