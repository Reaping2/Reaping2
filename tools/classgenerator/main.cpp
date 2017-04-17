#include "platform/i_platform.h"
#include <boost/program_options.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>
#include <string>
#include <ctype.h>
#include "boost/algorithm/string/find.hpp"
#include "boost/range/iterator_range_core.hpp"
#include <algorithm>


class Generator;
class IComponentGenerator;
class ComponentGenerator;
class NormalItemGenerator;
class SystemGenerator;
class CollisionSubSystemGenerator;
class ControllerSubSystemGenerator;
class NormalItemSubSystemGenerator;
class EventGenerator;
class MessageGenerator;
class FactoryGenerator;
class BuffGenerator;
class BuffSubSystemGenerator;
class MapElementGenerator;
class MapElementSystemGenerator;
class RecognizerGenerator;
class ActionRendererGenerator;
class EnumGenerator;
class RepositoryGenerator;
class WeaponGenerator;

class GeneratorFactory : public platform::Factory<Generator>, public platform::Singleton<GeneratorFactory>
{
    friend class platform::Singleton<GeneratorFactory>;
    template<typename Elem_T>
    static std::auto_ptr<Generator> CreateGenerator(int32_t Id);

    GeneratorFactory()
    {
        Bind(AutoId("default_generator"), &CreateGenerator<Generator>);
        SetDefault(AutoId("default_generator"));
        Bind(AutoId("i_component"), &CreateGenerator<IComponentGenerator>);
        Bind(AutoId("component"), &CreateGenerator<ComponentGenerator>);
        Bind(AutoId("normal_item"), &CreateGenerator<NormalItemGenerator>);
        Bind(AutoId("system"), &CreateGenerator<SystemGenerator>);
        Bind(AutoId("collision_sub_system"), &CreateGenerator<CollisionSubSystemGenerator>);
        Bind(AutoId("controller_sub_system"), &CreateGenerator<ControllerSubSystemGenerator>);
        Bind(AutoId("normal_item_sub_system"), &CreateGenerator<NormalItemSubSystemGenerator>);
        Bind(AutoId("event"), &CreateGenerator<EventGenerator>);
        Bind(AutoId("message"), &CreateGenerator<MessageGenerator>);
        Bind(AutoId("factory"), &CreateGenerator<FactoryGenerator>);
        Bind(AutoId("buff"), &CreateGenerator<BuffGenerator>);
        Bind(AutoId("buff_sub_system"), &CreateGenerator<BuffSubSystemGenerator>);
        Bind(AutoId("map_element"), &CreateGenerator<MapElementGenerator>);
        Bind(AutoId("map_element_system"), &CreateGenerator<MapElementSystemGenerator>);
        Bind(AutoId("recognizer"), &CreateGenerator<RecognizerGenerator>);
        Bind(AutoId("action_renderer"), &CreateGenerator<ActionRendererGenerator>);
        Bind(AutoId("enum"), &CreateGenerator<EnumGenerator>);
        Bind(AutoId("repository"), &CreateGenerator<RepositoryGenerator>);
        Bind(AutoId("weapon"), &CreateGenerator<WeaponGenerator>);
    }
};

template<typename Elem_T>
std::auto_ptr<Generator> GeneratorFactory::CreateGenerator(int32_t Id)
{
    std::auto_ptr<Generator> generator(new Elem_T());
    return generator;
}

class Generator
{
public:
    typedef std::pair<std::string, std::string> Type_Member_Pair_t;
    typedef std::vector<Type_Member_Pair_t> Type_Member_Pairs_t;
    typedef std::pair<std::string, std::string> Namespace_Event_Pair_t;
    typedef std::vector<Namespace_Event_Pair_t> Namespace_Event_Pairs_t;
    std::string classUnderscore;
    std::string parentUnderscore;
    std::string namespaceLowerCase;
    std::string membersArg;
    std::string eventsArg;
    std::string targetUnderscore;
    std::string targetItemTypeUnderscore;
    std::string targetItemNameUnderscore;
    std::string directoryName;
    std::string command;
protected:
    std::string classCamelCase;
    std::string classUpperCaseUnderscore;
    std::string classVariable;
    std::string parentCamelCase;
    std::string nameSpaceUpperCase;
    std::string targetCamelCase;
    std::string targetVariableName;
    std::string targetItemTypeCamelCase;
    std::string targetItemTypeVariableName;
    std::string targetItemNameCamelCase;
    std::string targetItemNameVariableName;
    std::string headerGuard;
    Type_Member_Pairs_t typeMemberPairs;
    Namespace_Event_Pairs_t namespaceEventPairs;
public:
    void WriteCommand( AutoNormalFile& file )
    {
        fprintf( file.mFile, "\n//command: %s\n", command.c_str() );
    }
    virtual void Generate()
    {
        L1( "default generator implementation, do nothing\n" );
    }
    virtual void Init()
    {
        classCamelCase = UnderscoreToCamelCase( classUnderscore );
        classVariable = classCamelCase;
        classVariable[0] = tolower( classCamelCase[0] );
        classUpperCaseUnderscore = boost::to_upper_copy( classUnderscore );
        parentCamelCase = UnderscoreToCamelCase( parentUnderscore );
        nameSpaceUpperCase = boost::to_upper_copy( namespaceLowerCase );
        if ( !targetUnderscore.empty() )
        {
            targetCamelCase = UnderscoreToCamelCase( targetUnderscore );
            targetVariableName = targetCamelCase;
            targetVariableName[0] = tolower( targetVariableName[0] );
        }

        if ( !targetItemTypeUnderscore.empty() )
        {
            targetItemTypeCamelCase = UnderscoreToCamelCase( targetItemTypeUnderscore );
            targetItemTypeVariableName = targetItemTypeCamelCase;
            targetItemTypeVariableName[0] = tolower( targetItemTypeVariableName[0] );
        }
        if ( !targetItemNameUnderscore.empty() )
        {
            targetItemNameCamelCase = UnderscoreToCamelCase( targetItemNameUnderscore );
            targetItemNameVariableName = targetItemNameCamelCase;
            targetItemNameVariableName[0] = tolower( targetItemNameVariableName[0] );
        }


        headerGuard = "INCLUDED_" + nameSpaceUpperCase + "_" + classUpperCaseUnderscore + "_H";
        typeMemberPairs = SplitMemberPairs( membersArg );
        namespaceEventPairs = SplitEventPairs( eventsArg );
    }
    virtual ~Generator() {}
    std::string UnderscoreToCamelCase( std::string underscore )
    {
        if ( underscore.empty() )
        {
            return std::string();
        }
        boost::to_lower( underscore );
        std::vector<std::string> parts;
        boost::split( parts, underscore, boost::is_any_of( "_" ) );
        for( std::vector<std::string>::iterator i = parts.begin(), e = parts.end(); i != e; ++i )
        {
            ( *i )[0] = toupper( ( *i )[0] );
        }
        std::string camelCase;
        camelCase = boost::join( parts, camelCase );
        return camelCase;
    }
    std::string CamelCaseToUnderscore(std::string camelCase)
    {
        if (camelCase.empty())
        {
            return std::string();
        }
        std::string underscore;
        bool first = true;
        for (auto c : camelCase)
        {
            if (!first&&isupper(c))
            {
                underscore = underscore + "_" + char(tolower(c));
            }
            else
            {
                underscore = underscore + char(tolower(c));
            }
            first = false;
        }
        return underscore;
    }
    Type_Member_Pairs_t SplitMemberPairs( std::string membersArg )
    {
        if ( membersArg.empty() )
        {
            return Type_Member_Pairs_t();
        }
        std::vector<std::string> typeMemberSplit;
        boost::split( typeMemberSplit, membersArg, boost::is_any_of( " " ) );

        Type_Member_Pairs_t typeMemberPairs;
        for( std::vector<std::string>::iterator i = typeMemberSplit.begin(), e = typeMemberSplit.end(); i != e; ++i )
        {
            std::vector<std::string> parts;
            boost::split( parts, *i, boost::is_any_of( "-" ) );
            if( parts.size() == 2 )
            {
                typeMemberPairs.push_back( Type_Member_Pair_t( parts[0], parts[1] ) );
            }
            else
            {
                L1( "members are not correctly separated: %s\n", i->c_str() );
            }
        }
        return typeMemberPairs;
    }

    Namespace_Event_Pairs_t SplitEventPairs( std::string eventsArg )
    {
        if ( eventsArg.empty() )
        {
            return Namespace_Event_Pairs_t();
        }
        std::vector<std::string> namespaceEventSplit;
        boost::split( namespaceEventSplit, eventsArg, boost::is_any_of( " " ) );

        Type_Member_Pairs_t namespaceEventPairs;
        for( std::vector<std::string>::iterator i = namespaceEventSplit.begin(), e = namespaceEventSplit.end(); i != e; ++i )
        {
            std::vector<std::string> parts;
            boost::split( parts, *i, boost::is_any_of( "-" ) );
            if( parts.size() == 2 )
            {
                namespaceEventPairs.push_back( Type_Member_Pair_t( parts[0], parts[1] ) );
            }
            else
            {
                L1( "events are not correctly separated: %s\n", i->c_str() );
            }
        }
        return namespaceEventPairs;
    }

    // from targetActor -> mTargetActor
    std::string CreateMemberName( std::string memberName )
    {
        memberName[0] = toupper( memberName[0] );
        std::string r = "m" + memberName;
        return r;
    }

    // from damageTaken -> mOnDamageTaken
    std::string VariableToCamelCase( std::string variableName )
    {
        variableName[0] = toupper( variableName[0] );
        return variableName;
    }

    // from targetActor -> GetTargetActor
    std::string CreateGetMember( std::string memberType, std::string memberName )
    {
        memberName[0] = toupper( memberName[0] );
        std::string r = ( memberType == "bool" ? "Is" : "Get" ) + memberName;
        return r;
    }
    // from targetActor -> SetTargetActor
    std::string CreateSetMember( std::string memberName )
    {
        memberName[0] = toupper( memberName[0] );
        std::string r = "Set" + memberName;
        return r;
    }
    // from int32_t,targetActor -> virtual int32_t GetTargetActor()const
    std::string CreateGetMemberFull( std::string memberType, std::string memberName )
    {
        std::string r = memberType + " " + CreateGetMember( memberType, memberName ) + "() const";
        return r;
    }
    // from int32_t,targetActor -> virtual void SetTargetActor(int32_t targetActor)
    std::string CreateSetMemberFull( std::string memberType, std::string memberName )
    {
        std::string r = "void " + CreateSetMember( memberName ) + "( " + memberType + " " + memberName + " )";
        return r;
    }
    // from int32_t,targetActor -> virtual int32_t GetTargetActor()const
    std::string CreateVirtualGetMemberFull( std::string memberType, std::string memberName )
    {
        std::string r = "virtual " + CreateGetMemberFull( memberType, memberName );
        return r;
    }
    // from int32_t,targetActor -> virtual void SetTargetActor(int32_t targetActor)
    std::string CreateVirtualSetMemberFull( std::string memberType, std::string memberName )
    {
        std::string r = "virtual " + CreateSetMemberFull( memberType, memberName );
        return r;
    }

    /// from int32_t,targetActor -> virtual int32_t GetTargetActor()const=0
    std::string CreateAbstractGetMember( std::string memberType, std::string memberName )
    {
        std::string r = "virtual " + memberType + " " + CreateGetMember( memberType, memberName ) + "()const=0";
        return r;
    }

    /// from int32_t,targetActor -> virtual void SetTargetActor(int32_t targetActor)=0
    std::string CreateAbstractSetMember( std::string memberType, std::string memberName )
    {
        std::string r = "virtual void " + CreateSetMember( memberName ) + "(" + memberType + " " + memberName + ")=0";
        return r;
    }

    // from int32_t,targetActor -> int32_t mTargetActor
    std::string CreateMemberWithType( std::string memberType, std::string memberName )
    {
        std::string r = memberType + " " + CreateMemberName( memberName );
        return r;
    }

    // from int32_t,targetActor -> int32_t targetActor
    std::string CreateArgumentWithType( std::string memberType, std::string memberName )
    {
        std::string r = memberType + " " + memberName;
        return r;
    }

    // from int32_t,targetActor,TargetComponent ->
    // int32_t GetTargetActor() const
    // {
    //     return mTargetActor;
    // }
    //
    std::string CreateGetMemberCppDefiniton( std::string memberType, std::string memberName, std::string ClassCamelCase )
    {

        std::string r = memberType + " " + ClassCamelCase + "::" + CreateGetMember( memberType, memberName ) + "() const\n";
        r = r + "{\n";
        r = r + "    return " + CreateMemberName( memberName ) + ";\n";
        r = r + "}\n";
        r = r + "\n";
        return r;
    }

    // from int32_t,targetActor,TargetComponent ->
    // virtual void SetTargetActor(int32_t targetActor)
    // {
    //     mTargetActor=targetActor;
    // }
    //
    std::string CreateSetMemberCppDefiniton( std::string memberType, std::string memberName, std::string ClassCamelCase )
    {
        std::string r = "void " + ClassCamelCase + "::" + CreateSetMember( memberName ) + "( " + memberType + " " + memberName + " )\n";
        r = r + "{\n";
        r = r + "    " + CreateMemberName( memberName ) + " = " + memberName + ";\n";
        r = r + "}\n";
        r = r + "\n";
        return r;
    }
};



class IComponentGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "IComponentGenerator started\n" );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "component";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "core";
        }
        Init();
        boost::filesystem::path dir( directoryName );
        boost::filesystem::create_directory( dir );
        {
            AutoNormalFile file( (directoryName + "/" + classUnderscore + ".h").c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"component.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_COMPONENT_BASE(%s)\n", classCamelCase.c_str() );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateAbstractSetMember( i->first, i->second ).c_str() );
                fprintf( file.mFile, "    %s;\n", CreateAbstractGetMember( i->first, i->second ).c_str() );
            }
            fprintf( file.mFile, "public:\n");
            fprintf( file.mFile, "    friend class ::boost::serialization::access;\n");
            fprintf( file.mFile, "    template<class Archive>\n");
            fprintf( file.mFile, "    void serialize( Archive& ar, const unsigned int version );\n");
            fprintf( file.mFile, "};\n");
            fprintf( file.mFile, "\n");

            fprintf( file.mFile, "template<class Archive>\n");
            fprintf( file.mFile, "void %s::serialize(Archive& ar, const unsigned int version)\n", classCamelCase.c_str());
            fprintf( file.mFile, "{\n");
            fprintf( file.mFile, "    ar& boost::serialization::base_object<Component>(*this);\n" );
            fprintf( file.mFile, "}\n");
            fprintf( file.mFile, "\n");
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );
            WriteCommand( file );

        }
        L1( "IComponentGenerator ended\n" );
    }
};

class ComponentGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "ComponentGenerator started\n" );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "i_" + classUnderscore;
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "core";
        }

        Init();
        boost::filesystem::path dir( directoryName );
        boost::filesystem::create_directory( dir );
        {
            AutoNormalFile file( (directoryName + "/" + classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"%s.h\"\n", parentUnderscore.c_str() );
            fprintf( file.mFile, "#include \"core/property_loader.h\"\n" );
            fprintf( file.mFile, "#include \"platform/export.h\"\n");
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateVirtualSetMemberFull( i->first, i->second ).c_str() );
                fprintf( file.mFile, "    %s;\n", CreateVirtualGetMemberFull( i->first, i->second ).c_str() );
            }
            fprintf( file.mFile, "protected:\n" );
            fprintf( file.mFile, "    friend class ComponentFactory;\n" );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateMemberWithType( i->first, i->second ).c_str() );
            }
            fprintf( file.mFile, "public:\n");
            fprintf( file.mFile, "    friend class ::boost::serialization::access;\n");
            fprintf( file.mFile, "    template<class Archive>\n");
            fprintf( file.mFile, "    void serialize( Archive& ar, const unsigned int version );\n");
            fprintf( file.mFile, "};\n");
            fprintf( file.mFile, "\n");

            fprintf( file.mFile, "template<class Archive>\n");
            fprintf( file.mFile, "void %s::serialize(Archive& ar, const unsigned int version)\n", classCamelCase.c_str());
            fprintf( file.mFile, "{\n");
            fprintf( file.mFile, "    ar& boost::serialization::base_object<%s>(*this);\n", parentCamelCase.c_str());
            for (Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i)
            {
                fprintf( file.mFile, "    ar& %s;\n", CreateMemberName(i->second).c_str());
            }
            fprintf( file.mFile, "}\n");
            fprintf( file.mFile, "\n");

            fprintf( file.mFile, "class %sLoader : public ComponentLoader<%s>\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    virtual void BindValues();\n" );
            fprintf( file.mFile, "protected:\n" );
            fprintf( file.mFile, "    %sLoader();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    friend class ComponentLoaderFactory;\n" );
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "REAPING2_CLASS_EXPORT_KEY2( %s, %s, \"%s\" );\n", classCamelCase.c_str(), classCamelCase.c_str(), classUnderscore.c_str());
            fprintf( file.mFile, "\n");

            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "//TODO: to component_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s\"), &CreateComponent<%s>);\n", classUnderscore.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "//TODO: to component_loader_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s\"), &CreateComponentLoader<%sLoader>);\n", classUnderscore.c_str(), classCamelCase.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( directoryName + "/" + classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"core/%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            bool isFirst = true;
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s %s(_fill_me_)\n",
                         isFirst ? ":" : ",", CreateMemberName( i->second ).c_str() );
                isFirst = false;
            }
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "%s", CreateSetMemberCppDefiniton( i->first, i->second, classCamelCase ).c_str() );
                fprintf( file.mFile, "%s", CreateGetMemberCppDefiniton( i->first, i->second, classCamelCase ).c_str() );
            }

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %sLoader::BindValues()\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%sLoader::%sLoader()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n");
            fprintf( file.mFile, "\n");
            fprintf( file.mFile, "REAPING2_CLASS_EXPORT_IMPLEMENT( %s, %s );\n", classCamelCase.c_str(), classCamelCase.c_str());
        }
        {
            std::auto_ptr<Generator> generator(GeneratorFactory::Get()(AutoId("i_component")));
            generator->command = command;
            generator->classUnderscore = parentUnderscore;
            generator->parentUnderscore = "component";
            generator->namespaceLowerCase = namespaceLowerCase;
            generator->membersArg = membersArg;
            generator->eventsArg = eventsArg;
            generator->targetUnderscore = targetUnderscore;
            generator->targetItemTypeUnderscore = targetItemTypeUnderscore;
            generator->targetItemNameUnderscore = targetItemNameUnderscore;
            generator->directoryName = directoryName;
            generator->Generate();
        }
        {
            std::auto_ptr<Generator> generator(GeneratorFactory::Get()(AutoId("system")));
            generator->command = command;
            generator->classUnderscore = classUnderscore.substr( 0, classUnderscore.find( "_component" ) ) +"_system";
            generator->parentUnderscore = "";
            generator->namespaceLowerCase = "engine";
            generator->membersArg = membersArg;
            generator->eventsArg = eventsArg;
            generator->targetUnderscore = classUnderscore.substr(0,classUnderscore.find("_component"));
            generator->targetItemTypeUnderscore = targetItemTypeUnderscore;
            generator->targetItemNameUnderscore = targetItemNameUnderscore;
            generator->directoryName = directoryName;
            generator->Generate();
        }
        {
            AutoNormalFile file( (directoryName + "/autoids").c_str(), "w" );
            fprintf( file.mFile, "%s\n", classUnderscore.c_str() );
            fprintf( file.mFile, "%s\n", parentUnderscore.c_str() );
            fprintf( file.mFile, "%s_system\n", classUnderscore.c_str() );
            fprintf( file.mFile, "%s\n", classCamelCase.c_str() );
            fprintf( file.mFile, "%s\n", parentCamelCase.c_str() );
            fprintf( file.mFile, "%sSystem\n", classCamelCase.c_str() );
        }
        L1("ComponentGenerator ended\n");
    }
};

class MapElementGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "map_element";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "map";
        }

        Init();
        {
            AutoNormalFile file( ( classUnderscore + "_map_element.h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"core/map/%s.h\"\n", parentUnderscore.c_str() );
            fprintf( file.mFile, "#include \"platform/i_platform.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %sMapElement : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_MAP_ELEMENT_BASE(%sMapElement)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %sMapElement( int32_t Id );\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    void Load( Json::Value const& setters );\n" );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateSetMemberFull( i->first, i->second ).c_str() );
                fprintf( file.mFile, "    %s;\n", CreateGetMemberFull( i->first, i->second ).c_str() );
            }
            fprintf( file.mFile, "private:\n" );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateMemberWithType( i->first, i->second ).c_str() );
            }
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "//TODO: to map_element_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s\"), &CreateMapElement<%sMapElement>);\n", classUnderscore.c_str(), classCamelCase.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + "_map_element.cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"core/map/%s_map_element.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%sMapElement::%sMapElement( int32_t Id )\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : MapElement( Id )\n" );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    , %s( _fill_me_ )\n",
                         CreateMemberName( i->second ).c_str() );
            }
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %sMapElement::Load( Json::Value const& setters )\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    MapElement::Load( setters );\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "%s", CreateSetMemberCppDefiniton( i->first, i->second, classCamelCase + "MapElement" ).c_str() );
                fprintf( file.mFile, "%s", CreateGetMemberCppDefiniton( i->first, i->second, classCamelCase + "MapElement" ).c_str() );
            }
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
        }

        L1( "%s ended\n", __FUNCTION__ );
    }
};

class NormalItemGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "NormalItemGenerator started\n" );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "normal_item";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "core";
        }

        Init();
        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"normal_item.h\"\n" );
            fprintf( file.mFile, "#include \"core/property_loader.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    %s( int32_t id );\n", classCamelCase.c_str() );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateVirtualSetMemberFull( i->first, i->second ).c_str() );
                fprintf( file.mFile, "    %s;\n", CreateVirtualGetMemberFull( i->first, i->second ).c_str() );
            }
            fprintf( file.mFile, "protected:\n" );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateMemberWithType( i->first, i->second ).c_str() );
            }
            fprintf( file.mFile, "private:\n" );
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "//TODO: to item_factory.cpp:\n" );
            fprintf( file.mFile, "Bind<%s>(AutoId(\"%s\"));\n", classCamelCase.c_str(), classUnderscore.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"core/%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s( int32_t id )\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : NormalItem(id)\n" );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s %s(_fill_me_)\n",
                         ",", CreateMemberName( i->second ).c_str() );
            }
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "%s", CreateSetMemberCppDefiniton( i->first, i->second, classCamelCase ).c_str() );
                fprintf( file.mFile, "%s", CreateGetMemberCppDefiniton( i->first, i->second, classCamelCase ).c_str() );
            }

            fprintf( file.mFile, "\n" );
        }

        L1( "NormalItemGenerator ended\n" );
    }
};


class SystemGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "SystemGenerator started\n" );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "system";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "engine";
        }

        if ( targetUnderscore.empty() )
        {
            targetUnderscore = "some_target";
        }
        Init();
        boost::filesystem::path dir( directoryName );
        boost::filesystem::create_directory( dir );
        {
            AutoNormalFile file( (directoryName + "/" + classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"core/scene.h\"\n" );
            fprintf( file.mFile, "#include \"engine/%s.h\"\n", parentUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s%s\n"
                     , classCamelCase.c_str(), namespaceLowerCase == "engine" ? "" : "engine::", parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_SYSTEM_BASE(%s)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "protected:\n" );
            fprintf( file.mFile, "    virtual void Init();\n" );
            fprintf( file.mFile, "    virtual void Update( double DeltaTime );\n" );
            fprintf( file.mFile, "private:\n" );
            fprintf( file.mFile, "    Scene& mScene;\n" );
            for( Type_Member_Pairs_t::iterator i = namespaceEventPairs.begin(), e = namespaceEventPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    AutoReg mOn%s;\n", VariableToCamelCase( i->second ).c_str() );
                fprintf( file.mFile, "    void On%s(%s::%sEvent const& Evt);\n"
                         , VariableToCamelCase( i->second ).c_str(), i->first.c_str(), VariableToCamelCase( i->second ).c_str() );
            }
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "//TODO: to system_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s\"), &CreateSystem<%s::%s>);\n", classUnderscore.c_str(), namespaceLowerCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "//TODO: to main.cpp:\n" );
            fprintf( file.mFile, "Eng.AddSystem(AutoId(\"%s\"));\n", classUnderscore.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( (directoryName + "/" + classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"platform/i_platform.h\"\n" );
            fprintf( file.mFile, "#include \"%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "#include \"core/i_%s_component.h\"\n", targetUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : mScene( Scene::Get() )\n" );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Init()\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            for( Type_Member_Pairs_t::iterator i = namespaceEventPairs.begin(), e = namespaceEventPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    mOn%s=EventServer<%s::%sEvent>::Get().Subscribe( boost::bind( &%s::On%s, this, _1 ) );\n"
                         , VariableToCamelCase( i->second ).c_str(), i->first.c_str(), VariableToCamelCase( i->second ).c_str(), classCamelCase.c_str(), VariableToCamelCase( i->second ).c_str() );
            }
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {\n" );
            fprintf( file.mFile, "        return actor.Get<I%sComponent>().IsValid(); } );\n", targetCamelCase.c_str() );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Update(double DeltaTime)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    for ( auto actor : mScene.GetActorsFromMap( GetType_static() ) )\n" );
            fprintf( file.mFile, "    {\n" );
            fprintf( file.mFile, "        Opt<I%sComponent> %sC = actor->Get<I%sComponent>();\n", targetCamelCase.c_str(), targetVariableName.c_str(), targetCamelCase.c_str() );
            fprintf( file.mFile, "        if (!%sC.IsValid())\n", targetVariableName.c_str() );
            fprintf( file.mFile, "        {\n" );
            fprintf( file.mFile, "            continue;\n" );
            fprintf( file.mFile, "        }\n" );
            fprintf( file.mFile, "    }\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            for( Type_Member_Pairs_t::iterator i = namespaceEventPairs.begin(), e = namespaceEventPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "void %s::On%s(%s::%sEvent const& Evt)\n"
                         , classCamelCase.c_str(), VariableToCamelCase( i->second ).c_str(), i->first.c_str(), VariableToCamelCase( i->second ).c_str() );
                fprintf( file.mFile, "{\n" );
                fprintf( file.mFile, "}\n" );
                fprintf( file.mFile, "\n" );
            }

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
        }

        L1( "SystemGenerator ended\n" );
    }
};

class MapElementSystemGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "map_element_system";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "map";
        }

        if ( targetUnderscore.empty() )
        {
            targetUnderscore = "some_target";
        }
        Init();

        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"core/map/%s.h\"\n", parentUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s%s\n"
                     , classCamelCase.c_str(), namespaceLowerCase == "map" ? "" : "map::", parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_SYSTEM_BASE(%s)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "protected:\n" );
            fprintf( file.mFile, "    virtual void Init();\n" );
            fprintf( file.mFile, "    virtual void Update( double DeltaTime );\n" );
            fprintf( file.mFile, "private:\n" );
            for( Type_Member_Pairs_t::iterator i = namespaceEventPairs.begin(), e = namespaceEventPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    AutoReg mOn%s;\n", VariableToCamelCase( i->second ).c_str() );
                fprintf( file.mFile, "    void On%s(%s::%sEvent const& Evt);\n"
                         , VariableToCamelCase( i->second ).c_str(), i->first.c_str(), VariableToCamelCase( i->second ).c_str() );
            }
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "//TODO: to system_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s\"), &CreateSystem<%s::%s>);\n", classUnderscore.c_str(), namespaceLowerCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "//TODO: to main.cpp:\n" );
            fprintf( file.mFile, "Eng.AddSystem(AutoId(\"%s\"));\n", classUnderscore.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"platform/i_platform.h\"\n" );
            fprintf( file.mFile, "#include \"%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "#include \"core/map/%s.h\"\n", targetUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : %s()\n", parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Init()\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    %s::Init();\n", parentCamelCase.c_str() );
            for( Type_Member_Pairs_t::iterator i = namespaceEventPairs.begin(), e = namespaceEventPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    mOn%s=EventServer<%s::%sEvent>::Get().Subscribe( boost::bind( &%s::On%s, this, _1 ) );\n"
                         , VariableToCamelCase( i->second ).c_str(), i->first.c_str(), VariableToCamelCase( i->second ).c_str(), classCamelCase.c_str(), VariableToCamelCase( i->second ).c_str() );
            }
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Update(double DeltaTime)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    %s::Update( DeltaTime );\n", parentCamelCase.c_str() );
            fprintf( file.mFile, "    for( Opt<%s> %s : MapElementListFilter<MapSystem::All>( mMapSystem->GetMapElementList(), %s::GetType_static() ) )\n"
                , targetCamelCase.c_str(), targetVariableName.c_str(), targetCamelCase.c_str() );
            fprintf( file.mFile, "    {\n" );
            fprintf( file.mFile, "    }\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            for( Type_Member_Pairs_t::iterator i = namespaceEventPairs.begin(), e = namespaceEventPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "void %s::On%s(%s::%sEvent const& Evt)\n"
                         , classCamelCase.c_str(), VariableToCamelCase( i->second ).c_str(), i->first.c_str(), VariableToCamelCase( i->second ).c_str() );
                fprintf( file.mFile, "{\n" );
                fprintf( file.mFile, "}\n" );
                fprintf( file.mFile, "\n" );
            }

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
        }

        L1( "%s ended\n", __FUNCTION__ );
    }
};


class SubSystemGenerator : public Generator
{
protected:
    virtual void Init()
    {
        if ( targetUnderscore.empty() )
        {
            targetUnderscore = "some_target";
        }
        Generator::Init();
    }

    virtual void Generate()
    {
        L1( "SubSystemGenerator started\n" );
        L1( "SubSystemGenerator ended\n" );
    }
};



class CollisionSubSystemGenerator : public SubSystemGenerator
{
    virtual void Generate()
    {
        L1( "CollisionSubSystemGenerator started\n" );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "collision_sub_system";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "engine";
        }

        Init();

        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"engine/collisions/collision_sub_system.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "protected:\n" );
            fprintf( file.mFile, "    virtual void Init();\n" );
            fprintf( file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n" );
            fprintf( file.mFile, "    virtual void ClipScene( Actor& actor );\n" );
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "//TODO: to sub_system_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n", classUnderscore.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "//TODO: to main.cpp:\n" );
            fprintf( file.mFile, "collisionSS->AddSubSystem(AutoId(\"%s_collision_component\"),AutoId(\"%s\"));\n", targetUnderscore.c_str(), classUnderscore.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"platform/i_platform.h\"\n" );
            fprintf( file.mFile, "#include \"engine/engine.h\"\n" );
            fprintf( file.mFile, "#include \"engine/collisions/%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "#include \"core/%s_collision_component.h\"\n", targetUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : CollisionSubSystem()\n" );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Init()\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    if (!mOther)\n" );
            fprintf( file.mFile, "    {\n" );
            fprintf( file.mFile, "        return;\n" );
            fprintf( file.mFile, "    }\n" );
            fprintf( file.mFile, "    Opt<%sCollisionComponent> %sCC=actor.Get<ICollisionComponent>();\n", targetCamelCase.c_str(), targetVariableName.c_str() );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::ClipScene(Actor& actor)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    CollisionSubSystem::ClipScene(actor);\n" );
            fprintf( file.mFile, "    Opt<%sCollisionComponent> %sCC=actor.Get<ICollisionComponent>();\n", targetCamelCase.c_str(), targetVariableName.c_str() );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
        }

        L1( "CollisionSubSystemGenerator ended\n" );
    }
};

class ControllerSubSystemGenerator : public SubSystemGenerator
{
    virtual void Generate()
    {
        L1( "ControllerSubSystemGenerator started\n" );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "sub_system";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "engine";
        }

        Init();

        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"engine/sub_system.h\"\n" );
            fprintf( file.mFile, "#include \"core/scene.h\"\n" );
            fprintf( file.mFile, "#include \"core/program_state.h\"\n" );
            fprintf( file.mFile, "using core::ProgramState;\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "protected:\n" );
            fprintf( file.mFile, "    virtual void Init();\n" );
            fprintf( file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n" );
            fprintf( file.mFile, "private:\n" );
            fprintf( file.mFile, "    Scene& mScene;\n" );
            fprintf( file.mFile, "    ProgramState& mProgramState;\n" );
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "//TODO: to sub_system_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n", classUnderscore.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "//TODO: to main.cpp:\n" );
            fprintf( file.mFile, "controllserSystem->AddSubSystem(AutoId(\"%s_controller_component\"),AutoId(\"%s\"));\n", targetUnderscore.c_str(), classUnderscore.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"platform/i_platform.h\"\n" );
            fprintf( file.mFile, "#include \"engine/engine.h\"\n" );
            fprintf( file.mFile, "#include \"engine/controllers/%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "#include \"core/%s_controller_component.h\"\n", targetUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : mScene(Scene::Get())\n" );
            fprintf( file.mFile, "    , mProgramState(core::ProgramState::Get())\n" );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Init()\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    Opt<%sControllerComponent> %sCC=actor.Get<IControllerComponent>();\n", targetCamelCase.c_str(), targetVariableName.c_str() );
            fprintf( file.mFile, "    if (!%sCC.IsValid()||!%sCC->IsEnabled())\n", targetVariableName.c_str(), targetVariableName.c_str() );
            fprintf( file.mFile, "    {\n" );
            fprintf( file.mFile, "        return;\n" );
            fprintf( file.mFile, "    }\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
        }

        L1( "ControllerSubSystemGenerator ended\n" );
    }
};

class NormalItemSubSystemGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "NormalItemSubSystemGenerator started\n" );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "sub_system";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "engine";
        }
        if ( targetItemTypeUnderscore.empty() )
        {
            targetItemTypeUnderscore = "normal_item";
        }
        if ( targetItemNameUnderscore.empty() )
        {
            targetItemNameUnderscore = "item_name";
        }
        Init();

        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"engine/items/common_sub_system_includes.h\"\n" );
            fprintf( file.mFile, "#include \"%s_sub_system.h\"\n", targetItemTypeUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "protected:\n" );
            fprintf( file.mFile, "    virtual void Init();\n" );
            fprintf( file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n" );
            fprintf( file.mFile, "private:\n" );
            fprintf( file.mFile, "    Scene& mScene;\n" );
            fprintf( file.mFile, "    Opt<%sSubSystem> m%sSubSystem;\n", targetItemTypeCamelCase.c_str(), targetItemTypeCamelCase.c_str() );
            fprintf( file.mFile, "    ActorFactory& mActorFactory;\n" );
            fprintf( file.mFile, "    int32_t mProjectileId;\n" );
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "//TODO: to sub_system_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n", classUnderscore.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "//TODO: to main.cpp:\n" );
            fprintf( file.mFile, "%sSS->AddSubSystem(AutoId(\"%s_%s\"),AutoId(\"%s\"));\n", targetItemTypeVariableName.c_str(), targetItemNameUnderscore.c_str(), targetItemTypeUnderscore.c_str(), classUnderscore.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"engine/items/%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "#include \"%s_sub_system.h\"\n", targetItemTypeUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : mScene(Scene::Get())\n" );
            fprintf( file.mFile, "    , m%sSubSystem(%sSubSystem::Get())\n", targetItemTypeCamelCase.c_str(), targetItemTypeCamelCase.c_str() );
            fprintf( file.mFile, "    , mActorFactory(ActorFactory::Get())\n" );
            fprintf( file.mFile, "    , mProjectileId(AutoId(\"_fill_me_\"))\n" );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Init()\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();\n" );
            fprintf( file.mFile, "    Opt<%s> %s = inventoryC->GetSelected%s();\n", targetItemTypeCamelCase.c_str(), targetItemTypeVariableName.c_str(), targetItemTypeCamelCase.c_str() );
            fprintf( file.mFile, "    if (%s->IsUse())\n", targetItemTypeVariableName.c_str() );
            fprintf( file.mFile, "    {\n" );
            fprintf( file.mFile, "        //TODO: do some stuff with it\n" );
            fprintf( file.mFile, "        %s->SetConsumed(true);\n", targetItemTypeVariableName.c_str() );
            fprintf( file.mFile, "    }\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
        }

        L1( "NormalItemSubSystemGenerator ended\n" );
    }
};

class EventGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "event";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "engine";
        }
        Init();

        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"platform/event.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "struct %s : public platform::%s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );

            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateMemberWithType( i->first, i->second ).c_str() );
            }

            fprintf( file.mFile, "    %s(", classCamelCase.c_str() );
            if ( typeMemberPairs.size() > 0 )
            {
                for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
                {
                    if ( e - 1 != i )
                    {
                        fprintf( file.mFile, "%s, ", CreateArgumentWithType( i->first, i->second ).c_str() );
                    }
                    else
                    {
                        fprintf( file.mFile, "%s)\n", CreateArgumentWithType( i->first, i->second ).c_str() );
                    }
                }
                fprintf( file.mFile, "        :" );
                for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
                {
                    if ( e - 1 != i )
                    {
                        fprintf( file.mFile, "%s(%s),", CreateMemberName( i->second ).c_str(), i->second.c_str() );
                    }
                    else
                    {
                        fprintf( file.mFile, "%s(%s){}\n", CreateMemberName( i->second ).c_str(), i->second.c_str() );
                    }
                }
            }
            else
            {
                fprintf( file.mFile, "){}\n" );
            }

            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );
            WriteCommand( file );
        }

        L1( "%s ended\n", __FUNCTION__ );
    }
};

class MessageGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "network";
        }

        Init();
        directoryName = directoryName + "_message";
        boost::filesystem::path dir( directoryName );
        boost::filesystem::create_directory( dir );
        bool pending = parentUnderscore == "pending";
        {
            AutoNormalFile file( (directoryName + "/" + classUnderscore + "_message.h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"network/message.h\"\n" );
            fprintf( file.mFile, "#include \"network/message_handler_sub_system.h\"\n" );
            fprintf( file.mFile, "#include \"network/message_sender_system.h\"\n" );
            fprintf( file.mFile, "#include \"platform/export.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %sMessage : public Message\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    friend class ::boost::serialization::access;\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_MESSAGE_BASE(%sMessage)\n", classCamelCase.c_str() );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateMemberWithType( i->first, i->second ).c_str() );
            }

            fprintf( file.mFile, "    %sMessage()\n", classCamelCase.c_str() );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                if ( i == typeMemberPairs.begin() )
                {
                    fprintf( file.mFile, "        : %s(_fill_me_)\n", CreateMemberName( i->second ).c_str() );
                }
                else
                {
                    fprintf( file.mFile, "        , %s(_fill_me_)\n", CreateMemberName( i->second ).c_str() );
                }
            }
            fprintf( file.mFile, "    {\n" );
            fprintf( file.mFile, "    }\n" );
            fprintf( file.mFile, "    template<class Archive>\n" );
            fprintf( file.mFile, "    void serialize(Archive& ar, const unsigned int version);\n" );
            fprintf( file.mFile, "    bool operator==( %sMessage const& other );\n", classCamelCase.c_str() );

            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "template<class Archive>\n" );
            fprintf( file.mFile, "void %sMessage::serialize(Archive& ar, const unsigned int version)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    ar& boost::serialization::base_object<Message>(*this);\n" );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    ar& %s;\n", CreateMemberName( i->second ).c_str() );
            }
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );
            if ( pending )
            {
                fprintf( file.mFile, "class %sMessageHandlerSubSystem : public PendingMessageHandlerSubSystem<%sMessage>\n", classCamelCase.c_str(), classCamelCase.c_str() );
            }
            else
            {
                fprintf( file.mFile, "class %sMessageHandlerSubSystem : public MessageHandlerSubSystem\n", classCamelCase.c_str() );
            }
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%sMessageHandlerSubSystem)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %sMessageHandlerSubSystem();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    virtual void Init();\n" );
            if ( !pending )
            {
                fprintf( file.mFile, "    virtual void Execute(Message const& message );\n" );
            }
            fprintf( file.mFile, "    virtual void Update(double DeltaTime);\n" );
            if ( pending )
            {
                fprintf( file.mFile, "    virtual bool ProcessPending(Message const& message);\n" );
            }
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "class %sMessageSenderSystem : public MessageSenderSystem\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            for( Type_Member_Pairs_t::iterator i = namespaceEventPairs.begin(), e = namespaceEventPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    AutoReg mOn%s;\n", VariableToCamelCase( i->second ).c_str() );
                fprintf( file.mFile, "    void On%s(%s::%sEvent const& Evt);\n"
                         , VariableToCamelCase( i->second ).c_str(), i->first.c_str(), VariableToCamelCase( i->second ).c_str() );
            }
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_SYSTEM_BASE(%sMessageSenderSystem)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %sMessageSenderSystem();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    virtual void Init();\n" );
            fprintf( file.mFile, "    virtual void Update(double DeltaTime);\n" );
            if ( !targetUnderscore.empty() )
            {
                fprintf( file.mFile, "    static std::auto_ptr<%sMessage> Generate%sMessage(Actor &actor);\n", classCamelCase.c_str(), classCamelCase.c_str() );
            }
            fprintf( file.mFile, "};\n" );

            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "REAPING2_CLASS_EXPORT_KEY2( %s__%sMessage, %s::%sMessage, \"%s\" );\n"
                ,namespaceLowerCase.c_str(), classCamelCase.c_str(), namespaceLowerCase.c_str(), classCamelCase.c_str(), classUnderscore.c_str() );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "//TODO: to message_handler_sub_system_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s_message_handler_sub_system\"), &CreateSubSystem<%sMessageHandlerSubSystem>);\n", classUnderscore.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "//TODO: to system_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s_message_sender_system\"), &CreateSystem<%s::%sMessageSenderSystem>);\n", classUnderscore.c_str(), namespaceLowerCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "//TODO: to main.cpp:\n" );
            fprintf( file.mFile, "Eng.AddSystem(AutoId(\"%s_message_sender_system\"));\n", classUnderscore.c_str() );
            fprintf( file.mFile, "//TODO: to message_handler_sub_system_holder.cpp:\n" );
            fprintf( file.mFile, "messageHandlerSSH->AddSubSystem(%s::%sMessage::GetType_static(),AutoId(\"%s_message_handler_sub_system\"));\n",
                     namespaceLowerCase.c_str(), classCamelCase.c_str(), classUnderscore.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( (directoryName + "/" + classUnderscore + "_message.cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"platform/i_platform.h\"\n" );
            fprintf( file.mFile, "#include \"network/%s_message.h\"\n", classUnderscore.c_str() );
            if ( !targetUnderscore.empty() )
            {
                fprintf( file.mFile, "#include \"core/i_%s_component.h\"\n", targetUnderscore.c_str() );
            }

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "bool %sMessage::operator==( %sMessage const& other )\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            for (Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i)
            {
                if ( i == typeMemberPairs.begin() )
                {
                    fprintf( file.mFile, "    return %s == other.%s%s\n"
                        , CreateMemberName( i->second ).c_str(), CreateMemberName( i->second ).c_str()
                        , i == --typeMemberPairs.end() ? ";" : "" );
                }
                else
                {
                    fprintf( file.mFile, "        && %s == other.%s%s\n"
                        , CreateMemberName( i->second ).c_str(), CreateMemberName( i->second ).c_str()
                        , i == --typeMemberPairs.end() ? ";":"" );
                }
            }
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "%sMessageSenderSystem::%sMessageSenderSystem()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : MessageSenderSystem()\n" );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );
            L1( "%s ended1\n", __FUNCTION__ );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %sMessageSenderSystem::Init()\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    MessageSenderSystem::Init();\n" );
            for( Type_Member_Pairs_t::iterator i = namespaceEventPairs.begin(), e = namespaceEventPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    mOn%s=EventServer<%s::%sEvent>::Get().Subscribe( boost::bind( &%sMessageSenderSystem::On%s, this, _1 ) );\n"
                         , VariableToCamelCase( i->second ).c_str(), i->first.c_str(), VariableToCamelCase( i->second ).c_str(), classCamelCase.c_str(), VariableToCamelCase( i->second ).c_str() );
            }
            L1( "%s ended2\n", __FUNCTION__ );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %sMessageSenderSystem::Update(double DeltaTime)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    MessageSenderSystem::Update(DeltaTime);\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            L1( "%s ended3\n", __FUNCTION__ );
            for( Type_Member_Pairs_t::iterator i = namespaceEventPairs.begin(), e = namespaceEventPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "void %sMessageSenderSystem::On%s(%s::%sEvent const& Evt)\n"
                         , classCamelCase.c_str(), VariableToCamelCase( i->second ).c_str(), i->first.c_str(), VariableToCamelCase( i->second ).c_str() );
                fprintf( file.mFile, "{\n" );
                fprintf( file.mFile, "    std::auto_ptr<%sMessage> %sMsg(new %sMessage);\n", classCamelCase.c_str(), classVariable.c_str(), classCamelCase.c_str() );
                for( Type_Member_Pairs_t::iterator itTypeMember = typeMemberPairs.begin(), eitTypeMember = typeMemberPairs.end(); itTypeMember != eitTypeMember; ++itTypeMember )
                {
                    fprintf( file.mFile, "    %sMsg->%s=Evt.%s;\n", classVariable.c_str(), CreateMemberName( itTypeMember->second ).c_str(), CreateMemberName( itTypeMember->second ).c_str() );
                }
                fprintf( file.mFile, "    mMessageHolder.AddOutgoingMessage(%sMsg);\n", classVariable.c_str() );
                fprintf( file.mFile, "}\n" );
                fprintf( file.mFile, "\n" );
            }
            L1( "%s ended4\n", __FUNCTION__ );


            fprintf( file.mFile, "%sMessageHandlerSubSystem::%sMessageHandlerSubSystem()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            if ( pending )
            {
                fprintf( file.mFile, "    : PendingMessageHandlerSubSystem()\n" );
            }
            else
            {
                fprintf( file.mFile, "    : MessageHandlerSubSystem()\n" );
            }
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %sMessageHandlerSubSystem::Init()\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "void %sMessageHandlerSubSystem::Update(double DeltaTime)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            if ( pending )
            {
                fprintf( file.mFile, "    PendingMessageHandlerSubSystem::Update(DeltaTime);\n" );
            }
            else
            {
                fprintf( file.mFile, "    MessageHandlerSubSystem::Update(DeltaTime);\n" );
            }
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            L1( "%s ended5\n", __FUNCTION__ );
            if ( pending )
            {
                fprintf( file.mFile, "bool %sMessageHandlerSubSystem::ProcessPending(Message const& message)\n", classCamelCase.c_str() );
                fprintf( file.mFile, "{\n" );
                fprintf( file.mFile, "    %sMessage const& msg=static_cast<%sMessage const&>(message);\n", classCamelCase.c_str(), classCamelCase.c_str() );
                fprintf( file.mFile, "    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID); //guaranteed\n" );
                fprintf( file.mFile, "    L1(\"executing %%s: actorGUID %%d \\n\",__FUNCTION__,msg.mActorGUID );\n" );
                fprintf( file.mFile, "    \n" );
                fprintf( file.mFile, "    return true;\n" );
                fprintf( file.mFile, "}\n" );
            }
            else
            {
                fprintf( file.mFile, "void %sMessageHandlerSubSystem::Execute(Message const& message)\n", classCamelCase.c_str() );
                fprintf( file.mFile, "{\n" );
                fprintf( file.mFile, "    %sMessage const& msg=static_cast<%sMessage const&>(message);\n", classCamelCase.c_str(), classCamelCase.c_str() );
                fprintf( file.mFile, "    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);\n" );
                fprintf( file.mFile, "    if (!actor.IsValid())\n" );
                fprintf( file.mFile, "    {\n" );
                fprintf( file.mFile, "        L1(\"cannot find actor with GUID: (%%s) %%d \\n\",__FUNCTION__,msg.mActorGUID );\n" );
                fprintf( file.mFile, "        return;\n" );
                fprintf( file.mFile, "    }\n" );
                fprintf( file.mFile, "    \n" );
                fprintf( file.mFile, "}\n" );
            }
            fprintf( file.mFile, "\n" );
            if ( !targetUnderscore.empty() )
            {
                fprintf( file.mFile, "std::auto_ptr<%sMessage> %sMessageSenderSystem::Generate%sMessage(Actor &actor)\n", classCamelCase.c_str(), classCamelCase.c_str(), classCamelCase.c_str() );
                fprintf( file.mFile, "{\n" );
                fprintf( file.mFile, "    Opt<I%sComponent> %sC = actor.Get<I%sComponent>();\n", targetCamelCase.c_str(), targetVariableName.c_str(), targetCamelCase.c_str() );
                fprintf( file.mFile, "    if (!%sC.IsValid())\n", targetVariableName.c_str() );
                fprintf( file.mFile, "    {\n" );
                fprintf( file.mFile, "        return std::auto_ptr<%sMessage>();\n", classCamelCase.c_str() );
                fprintf( file.mFile, "    }\n" );
                fprintf( file.mFile, "    std::auto_ptr<%sMessage> %sMsg(new %sMessage);\n", classCamelCase.c_str(), classVariable.c_str(), classCamelCase.c_str() );
                for( Type_Member_Pairs_t::iterator itTypeMember = typeMemberPairs.begin(), eitTypeMember = typeMemberPairs.end(); itTypeMember != eitTypeMember; ++itTypeMember )
                {
                    if (itTypeMember->second == "actorGUID")
                    {
                        fprintf( file.mFile, "    %sMsg->%s=actor.GetGUID();\n", classVariable.c_str(), CreateMemberName( itTypeMember->second ).c_str() );
                    }
                    else
                    {
                        fprintf( file.mFile, "    %sMsg->%s=%sC->%s();\n", classVariable.c_str(), CreateMemberName( itTypeMember->second ).c_str(), targetVariableName.c_str(), CreateGetMember( itTypeMember->first, itTypeMember->second ).c_str() );
                    }
                }
                fprintf( file.mFile, "    return %sMsg;\n", classVariable.c_str() );
                fprintf( file.mFile, "}\n" );
                fprintf( file.mFile, "\n" );
            }
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "REAPING2_CLASS_EXPORT_IMPLEMENT( %s__%sMessage, %s::%sMessage );\n", namespaceLowerCase.c_str(), classCamelCase.c_str(), namespaceLowerCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "\n" );
        }
        {
            AutoNormalFile file( (directoryName + "/autoids").c_str(), "w" );
            fprintf( file.mFile, "%s_message\n", classUnderscore.c_str() );
            fprintf( file.mFile, "%s_message_sender_system\n", classUnderscore.c_str() );
            fprintf( file.mFile, "%s_message_handler_system\n", classUnderscore.c_str() );
            fprintf( file.mFile, "%sMessage\n", classCamelCase.c_str() );
            fprintf( file.mFile, "%sMessageSenderSystem\n", classCamelCase.c_str() );
            fprintf( file.mFile, "%sMessageHandlerSystem\n", classCamelCase.c_str() );
        }
        L1( "%s ended\n", __FUNCTION__ );
    }
};


class FactoryGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "core";
        }

        if ( targetUnderscore.empty() )
        {
            targetUnderscore = "some_target";
        }
        Init();

        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"platform/factory.h\"\n" );
            fprintf( file.mFile, "#include \"platform/singleton.h\"\n" );
            fprintf( file.mFile, "#include \"%s.h\"\n", targetUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public platform::Factory<%s>, public platform::Singleton<%s>\n", classCamelCase.c_str(), targetCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    friend class platform::Singleton<%s>;\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "protected:\n" );
            fprintf( file.mFile, "    template<typename Elem_T>\n" );
            fprintf( file.mFile, "    static std::auto_ptr<%s> Create%s( int32_t Id );\n", targetCamelCase.c_str(), targetCamelCase.c_str() );
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "template<typename Elem_T>\n" );
            fprintf( file.mFile, "std::auto_ptr<%s> %s::Create%s( int32_t Id )\n", targetCamelCase.c_str(), classCamelCase.c_str(), targetCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    return std::auto_ptr<%s>( new Elem_T() );\n", targetCamelCase.c_str() );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"platform/i_platform.h\"\n" );
            fprintf( file.mFile, "#include \"%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "using platform::AutoId;\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    Bind( AutoId(\"default_%s\"), &Create%s<Default%s> );\n", targetUnderscore.c_str(), targetCamelCase.c_str(), targetCamelCase.c_str() );
            fprintf( file.mFile, "    SetDefault( AutoId(\"default_%s\") );\n", targetUnderscore.c_str() );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
        }

        L1( "%s ended\n", __FUNCTION__ );
    }
};

class RepositoryGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "core";
        }

        if ( targetUnderscore.empty() )
        {
            targetUnderscore = "some_target";
        }
        Init();

        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"platform/repository.h\"\n" );
            fprintf( file.mFile, "#include \"platform/singleton.h\"\n" );
            fprintf( file.mFile, "#include \"%s.h\"\n", targetUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public platform::Repository<%s>, public platform::Singleton<%s>\n", classCamelCase.c_str(), targetCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    friend class platform::Singleton<%s>;\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    static %s const mDefault;\n", targetCamelCase.c_str() );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "};\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"platform/i_platform.h\"\n" );
            fprintf( file.mFile, "#include \"%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "using platform::AutoId;\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s const %s::mDefault = %s();\n", targetCamelCase.c_str(), classCamelCase.c_str(), targetCamelCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : Repository<%s>(mDefault)\n", targetCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "   //mElements.insert(_identifier_, new _target_)\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
        }

        L1( "%s ended\n", __FUNCTION__ );
    }
};

class BuffGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "buff";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "core";
        }

        Init();
        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"buff.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_BUFF_BASE(%s)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateVirtualSetMemberFull( i->first, i->second ).c_str() );
                fprintf( file.mFile, "    %s;\n", CreateVirtualGetMemberFull( i->first, i->second ).c_str() );
            }
            fprintf( file.mFile, "protected:\n" );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s;\n", CreateMemberWithType( i->first, i->second ).c_str() );
            }
            fprintf( file.mFile, "private:\n" );
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "//TODO: to buff_factory.cpp:\n" );
            fprintf( file.mFile, "Bind(AutoId(\"%s\"), &CreateBuff<%s> );\n", classUnderscore.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "//TODO: to message_order.h:\n" );
            fprintf( file.mFile, "type=%s::GetType_static();\n", classCamelCase.c_str() );
            WriteCommand( file );

        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"core/buffs/%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : Buff()\n" );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    %s %s(_fill_me_)\n",
                         ",", CreateMemberName( i->second ).c_str() );
            }
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    mSecsToEnd=0.0;\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "%s", CreateSetMemberCppDefiniton( i->first, i->second, classCamelCase ).c_str() );
                fprintf( file.mFile, "%s", CreateGetMemberCppDefiniton( i->first, i->second, classCamelCase ).c_str() );
            }

            fprintf( file.mFile, "\n" );
        }

        L1( "%s ended\n", __FUNCTION__ );
    }
};

class BuffSubSystemGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "sub_system";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "engine";
        }
        if ( targetUnderscore.empty() )
        {
            size_t pos = classUnderscore.find( "_sub_system" );
            if ( pos != std::string::npos )
            {
                targetUnderscore = classUnderscore.substr( 0, pos );
            }
            else
            {
                targetUnderscore = "some_target";
            }
        }
        Init();

        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"core/scene.h\"\n" );
            fprintf( file.mFile, "#include \"engine/sub_system.h\"\n" );
            fprintf( file.mFile, "#include \"core/program_state.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "protected:\n" );
            fprintf( file.mFile, "    virtual void Init();\n" );
            fprintf( file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n" );
            fprintf( file.mFile, "private:\n" );
            fprintf( file.mFile, "    Scene& mScene;\n" );
            fprintf( file.mFile, "    core::ProgramState& mProgramState;\n" );
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "//TODO: to sub_system_factory.cpp:\n" );
            fprintf( file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n", classUnderscore.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "//TODO: to main.cpp:\n" );
            fprintf( file.mFile, "buffHolderS->AddSubSystem(%s::GetType_static(),AutoId(\"%s\"));\n", targetCamelCase.c_str(), classUnderscore.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"engine/buffs_engine/%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "#include \"core/buffs/%s.h\"\n", targetUnderscore.c_str() );
            fprintf( file.mFile, "#include \"core/buffs/i_buff_holder_component.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : mScene(Scene::Get())\n" );
            fprintf( file.mFile, "    , mProgramState(core::ProgramState::Get())\n" );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Init()\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();\n" );
            fprintf( file.mFile, "    if (!buffHolderC.IsValid())\n" );
            fprintf( file.mFile, "    {\n" );
            fprintf( file.mFile, "        return;\n" );
            fprintf( file.mFile, "    }\n" );
            fprintf( file.mFile, "    BuffListFilter<IBuffHolderComponent::All> buffListFilter(buffHolderC->GetBuffList(),%s::GetType_static());\n", targetCamelCase.c_str() );
            fprintf( file.mFile, "    for( BuffListFilter<IBuffHolderComponent::All>::const_iterator %sIt = buffListFilter.begin(), %sE = buffListFilter.end(); %sIt != %sE; ++%sIt )\n", targetVariableName.c_str(), targetVariableName.c_str(), targetVariableName.c_str(), targetVariableName.c_str(), targetVariableName.c_str() );
            fprintf( file.mFile, "    {\n" );
            fprintf( file.mFile, "        Opt<%s> %s(*%sIt);\n", targetCamelCase.c_str(), targetVariableName.c_str(), targetVariableName.c_str() );
            fprintf( file.mFile, "    }\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
        }

        L1( "%s ended\n", __FUNCTION__ );
    }
};

class ActionRendererGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "action_renderer";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "render";
        }
        if ( targetUnderscore.empty() )
        {
            size_t pos = classUnderscore.find( "_action_renderer" );
            if ( pos != std::string::npos )
            {
                targetUnderscore = classUnderscore.substr( 0, pos );
            }
            else
            {
                targetUnderscore = "some_target";
            }
        }
        Init();

        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"platform/i_platform.h\"\n" );
            fprintf( file.mFile, "#include \"render/action_renderer.h\"\n" );
            fprintf( file.mFile, "#include \"core/actor.h\"\n" );
            fprintf( file.mFile, "#include \"renderable_sprite.h\"\n" );
            fprintf( file.mFile, "#include \"renderable_repo.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    int32_t m%sId;\n", targetCamelCase.c_str() );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    %s( int32_t Id );\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    virtual void Init(const Actor& actor);\n" );
            fprintf( file.mFile, "    virtual void FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites);\n" );
            fprintf( file.mFile, "private:\n" );
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "//TODO: to action_renderer_factory.cpp:\n" );
            fprintf( file.mFile, "Bind<%s>(AutoId(\"%s\"));\n", classCamelCase.c_str(), targetUnderscore.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"render/%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s(int32_t Id)\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : ActionRenderer(Id)\n" );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    m%sId=AutoId(\"%s\");\n", targetCamelCase.c_str(), targetUnderscore.c_str() );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::Init(Actor const& actor)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());\n" );
            fprintf( file.mFile, "    Sprite const& Spr=Sprites(m%sId);\n", targetCamelCase.c_str() );
            fprintf( file.mFile, "    if( Spr.IsValid() )\n" );
            fprintf( file.mFile, "    {\n" );
            fprintf( file.mFile, "        mSecsToEnd=Spr.GetSecsToEnd();\n" );
            fprintf( file.mFile, "    }\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "void %s::FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());\n" );
            fprintf( file.mFile, "    Sprite const& Spr=Sprites(m%sId);\n", targetCamelCase.c_str() );
            fprintf( file.mFile, "    if( Spr.IsValid() )\n" );
            fprintf( file.mFile, "    {\n" );
            fprintf( file.mFile, "        SpritePhase const& Phase = Spr( (int32_t)GetState() );\n" );
            fprintf( file.mFile, "        renderableSprites.push_back(\n" );
            fprintf( file.mFile, "            RenderableSprite( &actor, m%sId, &Spr, &Phase/*, color*/) );\n", targetCamelCase.c_str() );
            fprintf( file.mFile, "    }\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
        }

        L1( "%s ended\n", __FUNCTION__ );
    }
};

class RecognizerGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        if ( parentUnderscore.empty() )
        {
            parentUnderscore = "recognizer";
        }
        if ( namespaceLowerCase.empty() )
        {
            namespaceLowerCase = "render";
        }
        if ( targetUnderscore.empty() )
        {
            size_t pos = classUnderscore.find( "_recognizerr" );
            if ( pos != std::string::npos )
            {
                targetUnderscore = classUnderscore.substr( 0, pos );
            }
            else
            {
                targetUnderscore = "some_target";
            }
        }
        Init();

        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"platform/i_platform.h\"\n" );
            fprintf( file.mFile, "#include \"render/recognizer.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    %s( int32_t Id );\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    virtual bool Recognize(Actor const&);\n" );
            fprintf( file.mFile, "private:\n" );
            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "//TODO: to recognizer_factory.cpp:\n" );
            fprintf( file.mFile, "Bind<%s>(AutoId(\"%s\"));\n", classCamelCase.c_str(), targetUnderscore.c_str() );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"render/%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::%s(int32_t Id)\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    : Recognizer(Id)\n" );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "bool %s::Recognize(Actor const& actor)\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    return true;\n" );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );

            fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
            fprintf( file.mFile, "\n" );
        }

        L1( "%s ended\n", __FUNCTION__ );
    }
};

class EnumGenerator : public Generator
{
    virtual void Generate()
    {
        L1( "%s started\n", __FUNCTION__ );
        Init();
        {
            AutoNormalFile file( ( classUnderscore + ".h" ).c_str(), "w" );
            fprintf( file.mFile, "#ifndef %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "#define %s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "#include \"platform/singleton.h\"\n" );
            fprintf( file.mFile, "#include \"boost/bimap.hpp\"\n" );
            fprintf( file.mFile, "\n" );
            if ( !namespaceLowerCase.empty() )
            {
                fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
                fprintf( file.mFile, "\n" );
            }

            fprintf( file.mFile, "class %s : public platform::Singleton<%s>\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "protected:\n" );
            fprintf( file.mFile, "    friend class platform::Singleton<%s>;\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    %s();\n", classCamelCase.c_str() );
            fprintf( file.mFile, "public:\n" );
            fprintf( file.mFile, "    enum Type\n" );
            fprintf( file.mFile, "    {\n" );
            bool isFirst = true;
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "        %s%s,\n", i->first.c_str(), isFirst ? "=0" : "" );
                isFirst = false;
            }
            fprintf( file.mFile, "        Num_Classes\n" );
            fprintf( file.mFile, "    };\n" );
            fprintf( file.mFile, "    %s::Type operator()( int32_t Id ) const;\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    int32_t operator()( %s::Type type ) const;\n", classCamelCase.c_str() );

            fprintf( file.mFile, "private:\n" );
            fprintf( file.mFile, "    typedef boost::bimap<int32_t,%s::Type> IdTo%sMap_t;\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    IdTo%sMap_t mIdTo%sMap;\n", classCamelCase.c_str(), classCamelCase.c_str() );

            fprintf( file.mFile, "};\n" );
            fprintf( file.mFile, "\n" );

            if ( !namespaceLowerCase.empty() )
            {
                fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
                fprintf( file.mFile, "\n" );
            }

            fprintf( file.mFile, "#endif//%s\n", headerGuard.c_str() );
            fprintf( file.mFile, "\n" );
            WriteCommand( file );
        }


        {
            AutoNormalFile file( ( classUnderscore + ".cpp" ).c_str(), "w" );
            fprintf( file.mFile, "#include \"core/%s.h\"\n", classUnderscore.c_str() );
            fprintf( file.mFile, "#include \"platform/auto_id.h\"\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "using platform::AutoId;\n" );
            fprintf( file.mFile, "\n" );
            if ( !namespaceLowerCase.empty() )
            {
                fprintf( file.mFile, "namespace %s {\n", namespaceLowerCase.c_str() );
                fprintf( file.mFile, "\n" );
            }
            fprintf( file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            for( Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i )
            {
                fprintf( file.mFile, "    mIdTo%sMap.insert(IdTo%sMap_t::value_type(AutoId(\"%s\"),%s::%s));\n", classCamelCase.c_str(), classCamelCase.c_str(), i->second.c_str(), classCamelCase.c_str(), i->first.c_str() );
            }
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "%s::Type %s::operator()( int32_t Id ) const\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    IdTo%sMap_t::left_const_iterator i=mIdTo%sMap.left.find(Id);\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    BOOST_ASSERT(i!=mIdTo%sMap.left.end());\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    return (i!=mIdTo%sMap.left.end())?i->second:%s::%s;\n", classCamelCase.c_str(), classCamelCase.c_str(), typeMemberPairs.begin()->first.c_str() );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );
            fprintf( file.mFile, "int32_t %s::operator()( Type type ) const\n", classCamelCase.c_str() );
            fprintf( file.mFile, "{\n" );
            fprintf( file.mFile, "    IdTo%sMap_t::right_const_iterator i=mIdTo%sMap.right.find(type);\n", classCamelCase.c_str(), classCamelCase.c_str() );
            fprintf( file.mFile, "    BOOST_ASSERT(i!=mIdTo%sMap.right.end());\n", classCamelCase.c_str() );
            fprintf( file.mFile, "    return (i!=mIdTo%sMap.right.end())?i->second:%s::%s;\n", classCamelCase.c_str(), classCamelCase.c_str(), typeMemberPairs.begin()->first.c_str() );
            fprintf( file.mFile, "}\n" );
            fprintf( file.mFile, "\n" );
            if ( !namespaceLowerCase.empty() )
            {
                fprintf( file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str() );
                fprintf( file.mFile, "\n" );
            }

        }

        L1( "%s ended\n", __FUNCTION__ );
    }
};

class WeaponGenerator : public Generator
{
    virtual void Generate()
    {
        L1("%s started\n", __FUNCTION__);
        if (parentUnderscore.empty())
        {
            parentUnderscore = "weapon";
        }

        Init();
        boost::filesystem::path dir( directoryName );
        boost::filesystem::create_directory(dir);
        dir = boost::filesystem::path( directoryName + "/data");
        boost::filesystem::create_directory(dir);
        dir = boost::filesystem::path( directoryName + "/data/items");
        boost::filesystem::create_directory(dir);
        dir = boost::filesystem::path( directoryName + "/data/sprites");
        boost::filesystem::create_directory(dir);
        dir = boost::filesystem::path( directoryName + "/data/actors");
        boost::filesystem::create_directory(dir);
        {
            AutoNormalFile file( (directoryName + "/" + classUnderscore + ".h").c_str(), "w" );
            fprintf(file.mFile, "#ifndef %s\n", headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n", headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"core/weapon.h\"\n");
            fprintf(file.mFile, "#include \"platform/export.h\"\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    %s( int32_t Id );\n", classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n", classCamelCase.c_str());
            for (Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i)
            {
                fprintf(file.mFile, "    %s;\n", CreateSetMemberFull(i->first, i->second).c_str());
                fprintf(file.mFile, "    %s;\n", CreateGetMemberFull(i->first, i->second).c_str());
            }
            fprintf(file.mFile, "private:\n");
            for (Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i)
            {
                fprintf(file.mFile, "    %s;\n", CreateMemberWithType(i->first, i->second).c_str());
            }
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    friend class ::boost::serialization::access;\n");
            fprintf(file.mFile, "    template<class Archive>\n");
            fprintf(file.mFile, "    void serialize( Archive& ar, const unsigned int version );\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "template<class Archive>\n");
            fprintf(file.mFile, "void %s::serialize(Archive& ar, const unsigned int version)\n", classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    ar& boost::serialization::base_object<Weapon>(*this);\n");
            for (Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i)
            {
                fprintf(file.mFile, "    ar& %s;\n", CreateMemberName(i->second).c_str());
            }
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "class %sLoader : public ItemLoader<%s>\n", classCamelCase.c_str(), classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    virtual void BindValues();\n");
            fprintf(file.mFile, "    %sLoader();\n", classCamelCase.c_str());
            fprintf(file.mFile, "    friend class ItemLoaderFactory;\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "REAPING2_CLASS_EXPORT_KEY2( %s, %s, \"%s\" );\n",classCamelCase.c_str(), classCamelCase.c_str(), classUnderscore.c_str());
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "#endif//%s\n", headerGuard.c_str());

            fprintf(file.mFile, "//TODO: to item_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId( \"%s\" ), &CreateItem<%s> );\n", classUnderscore.c_str(), classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to item_loader_factory.cpp:\n");
            fprintf(file.mFile, "id = AutoId( \"%s\" ); mElements.insert( id, new %sLoader() );\n", classUnderscore.c_str(), classCamelCase.c_str());
            WriteCommand(file);
        }


        {
            AutoNormalFile file((directoryName + "/" + classUnderscore + ".cpp").c_str(), "w");
            fprintf(file.mFile, "#include \"core/%s.h\"\n", classUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s( int32_t Id )\n", classCamelCase.c_str(), classCamelCase.c_str());
            fprintf(file.mFile, "    : Weapon( Id )\n");
            for (Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i)
            {
                fprintf(file.mFile, "    , %s(_fill_me_)\n", CreateMemberName(i->second).c_str());
            }
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str());
            fprintf(file.mFile, "    : Weapon( -1 )\n");
            for (Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i)
            {
                fprintf(file.mFile, "    , %s(_fill_me_)\n", CreateMemberName(i->second).c_str());
            }
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            for (Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i)
            {
                fprintf(file.mFile, "%s", CreateSetMemberCppDefiniton(i->first, i->second, classCamelCase).c_str());
                fprintf(file.mFile, "%s", CreateGetMemberCppDefiniton(i->first, i->second, classCamelCase).c_str());
            }

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %sLoader::BindValues()\n", classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%sLoader::%sLoader()\n", classCamelCase.c_str(), classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    SetBase<WeaponLoader>();\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "REAPING2_CLASS_EXPORT_IMPLEMENT( %s, %s );\n", classCamelCase.c_str(), classCamelCase.c_str());
        }
        {
            AutoNormalFile file((directoryName + "/data/items/" + classUnderscore + ".json").c_str(), "w");
            fprintf(file.mFile, "[\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        \"name\":\"%s\",\n",classUnderscore.c_str());
            fprintf(file.mFile, "        \"set\":[{\n");
            fprintf(file.mFile, "            \"shoot_cooldown\":0.1,\n");
            fprintf(file.mFile, "            \"shoot_alt_cooldown\":0.1,\n");
            fprintf(file.mFile, "            \"bullets\":20,\n");
            fprintf(file.mFile, "            \"shot_cost\":1,\n");
            fprintf(file.mFile, "            \"shot_cost_alt\":1,\n");
            fprintf(file.mFile, "            \"reload_time\":1.0,\n");
            fprintf(file.mFile, "            \"static_reload\":0,\n");
            fprintf(file.mFile, "            \"scatter_increase\":0.1,\n");
            fprintf(file.mFile, "            \"scatter_alt_increase\":0.1,\n");
            fprintf(file.mFile, "            \"scatter_chill\":0.1,\n");
            fprintf(file.mFile, "            \"scatter_magic_number\":0.1,\n");
            for (Type_Member_Pairs_t::iterator i = typeMemberPairs.begin(), e = typeMemberPairs.end(); i != e; ++i)
            {
                fprintf(file.mFile, "            \"%s\":0.0,\n", CamelCaseToUnderscore(i->second).c_str());
            }
            fprintf(file.mFile, "            \"position_x\":0.1,\n");
            fprintf(file.mFile, "            \"position_y\":0.1\n");
            fprintf(file.mFile, "        }]\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "]\n");
        }
        {
            AutoNormalFile file((directoryName + "/data/actors/" + classUnderscore + ".json").c_str(), "w");
            fprintf(file.mFile, "[\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        \"name\":\"%s_projectile\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "        \"components\":\n");
            fprintf(file.mFile, "        [\n");
            fprintf(file.mFile, "            {\"name\":\"position_component\"},\n");
            fprintf(file.mFile, "            {\"name\":\"move_component\",\n");
            fprintf(file.mFile, "                \"set\":[{\"speed\":2000}]\n");
            fprintf(file.mFile, "            },\n");
            fprintf(file.mFile, "            {\"name\":\"renderable_component\",\n");
            fprintf(file.mFile, "                \"set\":[{\"layer\":\"creeps\"}]\n");
            fprintf(file.mFile, "            },\n");
            fprintf(file.mFile, "            {\"name\":\"shot_collision_component\",\n");
            fprintf(file.mFile, "                \"set\":[{\"class\":\"projectile\", \"radius\":10, \"damage\":10 }]\n");
            fprintf(file.mFile, "            },\n");
            fprintf(file.mFile, "            {\"name\":\"health_component\"},\n");
            fprintf(file.mFile, "            {\"name\":\"owner_component\"},\n");
            fprintf(file.mFile, "            {\"name\":\"remove_on_death_component\"}\n");
            fprintf(file.mFile, "        ]\n");
            fprintf(file.mFile, "    },\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        \"name\":\"%s_alt_projectile\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "        \"components\":\n");
            fprintf(file.mFile, "        [\n");
            fprintf(file.mFile, "            {\"name\":\"position_component\"},\n");
            fprintf(file.mFile, "            {\"name\":\"move_component\",\n");
            fprintf(file.mFile, "                \"set\":[{\"speed\":2000}]\n");
            fprintf(file.mFile, "            },\n");
            fprintf(file.mFile, "            {\"name\":\"renderable_component\",\n");
            fprintf(file.mFile, "                \"set\":[{\"layer\":\"creeps\"}]\n");
            fprintf(file.mFile, "            },\n");
            fprintf(file.mFile, "            {\"name\":\"shot_collision_component\",\n");
            fprintf(file.mFile, "                \"set\":[{\"class\":\"projectile\", \"radius\":10, \"damage\":10 }]\n");
            fprintf(file.mFile, "            },\n");
            fprintf(file.mFile, "            {\"name\":\"health_component\"},\n");
            fprintf(file.mFile, "            {\"name\":\"owner_component\"},\n");
            fprintf(file.mFile, "            {\"name\":\"remove_on_death_component\"}\n");
            fprintf(file.mFile, "        ]\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "]\n");
        }
        {
            AutoNormalFile file((directoryName + "/data/sprites/" + classUnderscore + ".json").c_str(), "w");
            fprintf(file.mFile, "[\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        \"texture_path\":\"textures/%s_weapon.png\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "        \"actor_visuals\":\n");
            fprintf(file.mFile, "        [{\n");
            fprintf(file.mFile, "            \"name\":\"player\",\n");
            fprintf(file.mFile, "            \"animations\":\n");
            fprintf(file.mFile, "            [\n");
            fprintf(file.mFile, "                {\n");
            fprintf(file.mFile, "                    \"name\":\"%s\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "                    \"w\":100,\n");
            fprintf(file.mFile, "                    \"h\":100,\n");
            fprintf(file.mFile, "                    \"scale\":1,\n");
            fprintf(file.mFile, "                    \"phases\":[{\"x\":0,\"y\":0}]\n");
            fprintf(file.mFile, "                }\n");
            fprintf(file.mFile, "            ]\n");
            fprintf(file.mFile, "        }]\n");
            fprintf(file.mFile, "    },\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        \"texture_path\":\"textures/%s_weapon.png\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "        \"actor_visuals\":\n");
            fprintf(file.mFile, "        [{\n");
            fprintf(file.mFile, "            \"name\":\"ctf_player\",\n");
            fprintf(file.mFile, "            \"animations\":\n");
            fprintf(file.mFile, "            [\n");
            fprintf(file.mFile, "                {\n");
            fprintf(file.mFile, "                    \"name\":\"%s\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "                    \"w\":100,\n");
            fprintf(file.mFile, "                    \"h\":100,\n");
            fprintf(file.mFile, "                    \"scale\":1,\n");
            fprintf(file.mFile, "                    \"phases\":[{\"x\":0,\"y\":0}]\n");
            fprintf(file.mFile, "                }\n");
            fprintf(file.mFile, "            ]\n");
            fprintf(file.mFile, "        }]\n");
            fprintf(file.mFile, "    },\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        \"texture_path\":\"textures/%s_pickup.png\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "        \"actor_visuals\":\n");
            fprintf(file.mFile, "        [{\n");
            fprintf(file.mFile, "            \"name\":\"%s\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "            \"animations\":\n");
            fprintf(file.mFile, "            [\n");
            fprintf(file.mFile, "                {\n");
            fprintf(file.mFile, "                    \"name\":\"idle\",\n");
            fprintf(file.mFile, "                    \"w\":32,\n");
            fprintf(file.mFile, "                    \"h\":32,\n");
            fprintf(file.mFile, "                    \"scale\":1,\n");
            fprintf(file.mFile, "                    \"phases\":[{\"x\":0,\"y\":0}]\n");
            fprintf(file.mFile, "                }\n");
            fprintf(file.mFile, "            ]\n");
            fprintf(file.mFile, "        }]\n");
            fprintf(file.mFile, "    },\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        \"texture_path\":\"textures/%s_projectile.png\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "        \"actor_visuals\":\n");
            fprintf(file.mFile, "        [{\n");
            fprintf(file.mFile, "            \"name\":\"%s_projectile\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "            \"animations\":\n");
            fprintf(file.mFile, "            [\n");
            fprintf(file.mFile, "                {\n");
            fprintf(file.mFile, "                    \"name\":\"move\",\n");
            fprintf(file.mFile, "                    \"w\":10,\n");
            fprintf(file.mFile, "                    \"h\":10,\n");
            fprintf(file.mFile, "                    \"scale\":1,\n");
            fprintf(file.mFile, "                    \"phases\":[{\"x\":0,\"y\":0}]\n");
            fprintf(file.mFile, "                }\n");
            fprintf(file.mFile, "            ]\n");
            fprintf(file.mFile, "        }]\n");
            fprintf(file.mFile, "    },\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        \"texture_path\":\"textures/%s_alt_projectile.png\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "        \"actor_visuals\":\n");
            fprintf(file.mFile, "        [{\n");
            fprintf(file.mFile, "            \"name\":\"%s_alt_projectile\",\n", classUnderscore.c_str());
            fprintf(file.mFile, "            \"animations\":\n");
            fprintf(file.mFile, "            [\n");
            fprintf(file.mFile, "                {\n");
            fprintf(file.mFile, "                    \"name\":\"move\",\n");
            fprintf(file.mFile, "                    \"w\":10,\n");
            fprintf(file.mFile, "                    \"h\":10,\n");
            fprintf(file.mFile, "                    \"scale\":1,\n");
            fprintf(file.mFile, "                    \"phases\":[{\"x\":0,\"y\":0}]\n");
            fprintf(file.mFile, "                }\n");
            fprintf(file.mFile, "            ]\n");
            fprintf(file.mFile, "        }]\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "]\n");
        }
        parentUnderscore = "sub_system";
        namespaceLowerCase = "engine";
        std::string originalClassUnderscore = classUnderscore;
        std::string originalClassCamelCase = classCamelCase;
        classUnderscore = classUnderscore + "_weapon_sub_system";
        Init();
        
        {
            AutoNormalFile file((directoryName + "/" + classUnderscore + ".h").c_str(), "w");
            fprintf(file.mFile, "#ifndef %s\n", headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n", headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"engine/items/common_sub_system_includes.h\"\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n", namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n", classCamelCase.c_str(), parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n", classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n", classCamelCase.c_str());
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n");
            fprintf(file.mFile, "private:\n");
            fprintf(file.mFile, "    Scene& mScene;\n");
            fprintf(file.mFile, "    core::ProgramState& mProgramState;\n");
            fprintf(file.mFile, "    Opt<WeaponItemSubSystem> mWeaponItemSubSystem;\n");
            fprintf(file.mFile, "    ActorFactory& mActorFactory;\n");
            fprintf(file.mFile, "    int32_t mProjectileId;\n");
            fprintf(file.mFile, "    int32_t mProjectileAltId;\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n", headerGuard.c_str());

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to sub_system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n", classUnderscore.c_str(), classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to main.cpp:\n");
            fprintf(file.mFile, "weaponItemSS->AddSubSystem(AutoId(\"%s\"),AutoId(\"%s\"));\n", originalClassUnderscore.c_str(), classUnderscore.c_str());
            WriteCommand(file);
        }


        {
            AutoNormalFile file((directoryName + "/" + classUnderscore + ".cpp").c_str(), "w");
            fprintf(file.mFile, "#include \"engine/items/%s.h\"\n", classUnderscore.c_str());
            fprintf(file.mFile, "#include \"core/%s.h\"\n", originalClassUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n", namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n", classCamelCase.c_str(), classCamelCase.c_str());
            fprintf(file.mFile, "    : mScene(Scene::Get())\n");
            fprintf(file.mFile, "    , mProgramState(core::ProgramState::Get())\n");
            fprintf(file.mFile, "    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())\n");
            fprintf(file.mFile, "    , mActorFactory(ActorFactory::Get())\n");
            fprintf(file.mFile, "    , mProjectileId(AutoId(\"%s_projectile\"))\n",originalClassUnderscore.c_str());
            fprintf(file.mFile, "    , mProjectileAltId(AutoId(\"%s_alt_projectile\"))\n", originalClassUnderscore.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Init()\n", classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n", classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();\n");
            fprintf(file.mFile, "    Opt<%s> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );\n", originalClassCamelCase.c_str());
            fprintf(file.mFile, "    if (weapon->GetCooldown() > 0)\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        return;\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "    if ( weapon->IsShooting() )\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        WeaponItemSubSystem::Projectiles_t projectiles;\n");
            fprintf(file.mFile, "        std::auto_ptr<Actor> ps = mActorFactory( mProjectileId );\n");
            fprintf(file.mFile, "        projectiles.push_back( Opt<Actor>( ps.release() ) );\n");
            fprintf(file.mFile, "        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), false );\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "    else if ( weapon->IsShootingAlt() )\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        WeaponItemSubSystem::Projectiles_t projectiles;\n");
            fprintf(file.mFile, "        std::auto_ptr<Actor> ps = mActorFactory( mProjectileAltId );\n");
            fprintf(file.mFile, "        projectiles.push_back( Opt<Actor>( ps.release() ) );\n");
            fprintf(file.mFile, "        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), true );\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "} // namespace %s\n", namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }
        {
            AutoNormalFile file((directoryName + "/autoids").c_str(), "w");
            fprintf(file.mFile, "%s\n",originalClassUnderscore.c_str());
            fprintf(file.mFile, "%s_projectile\n", originalClassUnderscore.c_str());
            fprintf(file.mFile, "%s_alt_projectile\n", originalClassUnderscore.c_str());
            fprintf(file.mFile, "%s_weapon_sub_system\n", originalClassUnderscore.c_str());
            fprintf(file.mFile, "%sWeaponSubSystem\n", originalClassCamelCase.c_str());
        }

        L1("%s ended\n", __FUNCTION__);
    }
};


int main( int argc, char* argv[] )
{
    platform::IdStorage::Get().Init();
    namespace po = boost::program_options;
    // Declare the supported options.
    po::options_description desc( "Allowed options" );
    std::string classUnderscore;
    std::string generatorType;
    std::string parentUnderscore;
    std::string targetUnderscore;
    std::string targetItemTypeUnderscore;
    std::string targetItemNameUnderscore;
    std::string namespaceLowerCase;
    std::string membersArg;
    std::string eventsArg;
    desc.add_options()
    ( "help", "produce help message" )
    ( "-c", po::value<std::string>( &classUnderscore ), "class_name_underscore" )
    ( "-g", po::value<std::string>( &generatorType ), "type of generator" )
    ( "-p", po::value<std::string>( &parentUnderscore ), "parent_name_underscore" )
    ( "-t", po::value<std::string>( &targetUnderscore ), "target_name_underscore" )
    ( "target_item_type", po::value<std::string>( &targetItemTypeUnderscore ), "target_item_type" )
    ( "target_item_name", po::value<std::string>( &targetItemNameUnderscore ), "target_item_name" )
    ( "-n", po::value<std::string>( &namespaceLowerCase ), "namespacelowercase" )
    ( "-m", po::value<std::string>( &membersArg ), "optional: members: \"double-radius int32_t-targetId\"" )
    ( "-e", po::value<std::string>( &eventsArg ), "optional: events to subscribe: \"core-damageTaken\" -> AutoReg mOnDamageTaken; void OnDamageTaken (core::DamageTakenEvent const& Evt);" )
    ( "generators:",
      "\n*** action_renderer ***\n class_name shall be in \"{the_name_underscore}_action_renderer\" format. generates a class_name_underscore.h with constructor, base functions.\n )\n"
      "\n*** buff ***\n class_name shall be in \"{the_name_underscore}_buff\" format. generates a class_name_underscore.h and class_name_underscore.cpp with getters setters and member variables. guesses the parent to buff if not set. uses -m members \n"
      "\n*** buff_sub_system ***\n class_name shall be in \"{the_name_underscore}_buff_sub_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp. guesses the parent to buff if not set. \n"
      "\n*** collision_sub_system ***\n class_name shall be in \"{the_name_underscore}_collision_sub_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -t \"target_component_name_without_collision_component\" (e.g. for shot_collision_component \"shot\")\n"
      "\n*** component ***\n class_name shall be in \"{the_name_underscore}_component\" format. generates a class_name_underscore.h and class_name_underscore.cpp with getters setters and member variables. guesses the parent to i_class_name_underscore if not set \n"
      "\n*** controller_sub_system ***\n class_name shall be in \"{the_name_underscore}_controller_sub_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -t \"target_component_name_without_controller_component\" (e.g. for random_controller_component \"random\")\n"
      "\n*** default_generator ***\n does nothing\n"
      "\n*** event ***\n class_name shall be in \"{the_name_underscore}_event\" format. generates a class_name_underscore.h with constructor for memebers.\n  uses: -m members)\n"
      "\n*** enum ***\n class_name shall be in \"{the_name_underscore}\" format. generates a class_name_underscore.h with string to enum map.\n  uses: -m members (enum values paired with string values)\n"
      "\n*** factory ***\n class_name shall be in \"{the_name_underscore}_factory\" format. generates a class_name_underscore.h class_name_underscore.cpp.\n  uses: -t \"target_generated_class\" - base of the genereted classes by this factory)\n"
      "\n*** i_component ***\n class_name shall be in \"i_{the_name_underscore}_component\" format. generates a class_name_underscore.h with abstract member getters setters. guesses the parent to Component.\n"
      "\n*** message ***\n class_name shall be in \"{the_name_underscore}\" format. generates a class_name_message_underscore.h class_name_message_underscore.cpp with message, messagehandler, messagesender.\n  uses: -m members -e events -p pending (for delayed process) -t target_component_without_component (e.g i_move_fast_component->move_fast) for GenerateMessage static method\n"
      "\n*** map_element ***\n class_name shall be in \"{the_name_underscore}\" format (without _map_element). generates a class_name_underscore_map_element.h and class_name_underscore_map_element.cpp with getters setters and member variables. guesses the parent to map_element if not set. uses -m members \n"
      "\n*** map_element_system ***\n class_name shall be in \"{the_name_underscore}_map_element_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -t \"target_map_element_name\" uses -m members\n"
      "\n*** normal_item ***\n class_name shall be in \"{the_name_underscore}_normal_item\" format. generates a class_name_underscore.h and class_name_underscore.cpp with getters setters and member variables. guesses the parent to normal_item if not set. uses -m members \n"
      "\n*** normal_item_sub_system ***\n class_name shall be in \"{the_name_underscore}_normal_item_sub_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -target_item_type, -target_item_name (e.g. for grenade_normal_item -target_item_type \"narmal_item\" -target_item_name \"grenade\")\n"
      "\n*** recognizer ***\n class_name shall be in \"{the_name_underscore}_recognizer\" format. generates a class_name_underscore.h with constructor, base functions.\n )\n"
      "\n*** repository ***\n class_name shall be in \"{the_name_underscore}_repo\" format. generates a class_name_underscore.h class_name_underscore.cpp.\n  uses: -t \"target_class\" - base of the included classes by this repo) -n namespace\n"
      "\n*** system ***\n class_name shall be in \"{the_name_underscore}_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -t \"target_component_name_without_component\" (e.g. for drop_on_death_component: \"drop_on_death\")\n"
      "\n*** weapon ***\n class_name shall be in \"{the_name_underscore}\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods and a class_name_underscore.json.\n  uses: -m for members.\n"
        //"\n\n\n"
    )
    ;

    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );
    if ( vm.count( "help" ) )
    {
        std::cout << desc << "\n";
        return 1;
    }
    if ( !vm.count( "-g" ) )
    {
        L1( "need to specify a valid generator!" );
        return 2;
    }
    std::string command;
    for ( int i = 0; i < argc; ++i )
    {
        if ( argv[i][0] == '-' )
        {
            command = command + " " + std::string( argv[i] );
        }
        else
        {
            command = command + " \"" + std::string( argv[i] ) + "\"";
        }
    }
    L1( "//command: %s\n", command.c_str() );

    std::auto_ptr<Generator> generator( GeneratorFactory::Get()( AutoId( generatorType ) ) );
    generator->command = command;
    generator->classUnderscore = classUnderscore;
    generator->parentUnderscore = parentUnderscore;
    generator->namespaceLowerCase = namespaceLowerCase;
    generator->membersArg = membersArg;
    generator->eventsArg = eventsArg;
    generator->targetUnderscore = targetUnderscore;
    generator->targetItemTypeUnderscore = targetItemTypeUnderscore;
    generator->targetItemNameUnderscore = targetItemNameUnderscore;
    generator->directoryName = classUnderscore;
    generator->Generate();
    return 0;
}
