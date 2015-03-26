#include "platform/i_platform.h"
#include <boost/program_options.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>
#include <string>
#include <ctype.h>

class Generator
{
public:
    typedef std::pair<std::string,std::string> Type_Member_Pair_t;
    typedef std::vector<Type_Member_Pair_t> Type_Member_Pairs_t;
protected:
    std::string classCamelCase;
    std::string classUpperCaseUnderscore;
    std::string parentCamelCase;
    std::string nameSpaceUpperCase;
    std::string headerGuard;
    Type_Member_Pairs_t typeMemberPairs;
public:
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        L1("default generator implementation, do nothing\n");
    }
    virtual void Init(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        classCamelCase=UnderscoreToCamelCase(classUnderscore);
        parentCamelCase=UnderscoreToCamelCase(parentUnderscore);
        classUpperCaseUnderscore=boost::to_upper_copy(classUnderscore);
        nameSpaceUpperCase=boost::to_upper_copy(namespaceLowerCase);
        headerGuard="INCLUDED_"+nameSpaceUpperCase+"_"+classUpperCaseUnderscore+"_H";
        typeMemberPairs=SplitMemberPairs(membersArg);
    }
    virtual ~Generator() {}
    std::string UnderscoreToCamelCase(std::string underscore)
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

    Type_Member_Pairs_t SplitMemberPairs(std::string membersArg)
    {
        if (membersArg.empty())
        {
            return Type_Member_Pairs_t();
        }
        std::vector<std::string> typeMemberSplit;
        boost::split(typeMemberSplit,membersArg,boost::is_any_of(" "));

        Type_Member_Pairs_t typeMemberPairs;
        for(std::vector<std::string>::iterator i=typeMemberSplit.begin(),e=typeMemberSplit.end();i!=e;++i)
        {
            std::vector<std::string> parts;
            boost::split(parts,*i,boost::is_any_of("-"));
            if(parts.size()==2)
            {
                typeMemberPairs.push_back(Type_Member_Pair_t(parts[0],parts[1]));
            }
            else
            {
                L1("members are not correctly separated: %s",i->c_str());
            }
        }
        return typeMemberPairs;
    }

    // from targetActor -> mTargetActor
    std::string CreateMemberName(std::string memberName)
    {
        memberName[0]=toupper(memberName[0]);
        std::string r="m"+memberName;
        return r;
    }

    // from targetActor -> GetTargetActor
    std::string CreateGetMember(std::string memberType, std::string memberName)
    {
        memberName[0]=toupper(memberName[0]);
        std::string r=(memberType=="bool"?"Is":"Get")+memberName;
        return r;
    }
    // from targetActor -> SetTargetActor
    std::string CreateSetMember(std::string memberName)
    {
        memberName[0]=toupper(memberName[0]);
        std::string r="Set"+memberName;
        return r;
    }
    // from int32_t,targetActor -> virtual int32_t GetTargetActor()const
    std::string CreateGetMemberFull(std::string memberType, std::string memberName)
    {
        std::string r="virtual "+memberType+" "+CreateGetMember(memberType,memberName)+"()const";
        return r;
    }
    // from int32_t,targetActor -> virtual void SetTargetActor(int32_t targetActor)
    std::string CreateSetMemberFull(std::string memberType, std::string memberName)
    {
        std::string r="virtual void "+CreateSetMember(memberName)+"("+memberType+" "+memberName+")";
        return r;
    }

    /// from int32_t,targetActor -> virtual int32_t GetTargetActor()const=0
    std::string CreateAbstractGetMember(std::string memberType, std::string memberName)
    {
        std::string r="virtual "+memberType+" "+CreateGetMember(memberType,memberName)+"()const=0";
        return r;
    }

    /// from int32_t,targetActor -> virtual void SetTargetActor(int32_t targetActor)=0
    std::string CreateAbstractSetMember(std::string memberType, std::string memberName)
    {
        std::string r="virtual void "+CreateSetMember(memberName)+"("+memberType+" "+memberName+")=0";
        return r;
    }

    // from int32_t,targetActor -> int32_t mTargetActor
    std::string CreateMemberWithType(std::string memberType, std::string memberName)
    {
        std::string r=memberType+" "+CreateMemberName(memberName);
        return r;
    }

    // from int32_t,targetActor,TargetComponent -> 
    // int32_t GetTargetActor() const
    // {
    //     return mTargetActor;
    // }
    // 
    std::string CreateGetMemberCppDefiniton(std::string memberType, std::string memberName,std::string ClassCamelCase)
    {
        
        std::string r=memberType+" "+ClassCamelCase+"::"+CreateGetMember(memberType,memberName)+"()const\n";
        r=r+"{\n";
        r=r+"    return "+CreateMemberName(memberName)+";\n";
        r=r+"}\n";
        r=r+"\n";
        return r;
    }

    // from int32_t,targetActor,TargetComponent -> 
    // virtual void SetTargetActor(int32_t targetActor)
    // {
    //     mTargetActor=targetActor;
    // }
    // 
    std::string CreateSetMemberCppDefiniton(std::string memberType, std::string memberName,std::string ClassCamelCase)
    {
        std::string r="void "+ClassCamelCase+"::"+CreateSetMember(memberName)+"("+memberType+" "+memberName+")\n";
        r=r+"{\n";
        r=r+"    "+CreateMemberName(memberName)+"="+memberName+";\n";
        r=r+"}\n";
        r=r+"\n";
        return r;
    }
};

class IComponentGenerator : public Generator
{
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        L1("IComponentGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="component";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="core";
        }
        Init(classUnderscore,parentUnderscore,namespaceLowerCase, membersArg);

        {
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
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateAbstractSetMember(i->first,i->second).c_str());
                fprintf(file.mFile, "    %s;\n",CreateAbstractGetMember(i->first,i->second).c_str());
            }
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());
        }
        L1("IComponentGenerator ended\n");
    }
};

class ComponentGenerator : public Generator
{
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        L1("ComponentGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="i_"+classUnderscore;
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="core";
        }

        Init(classUnderscore,parentUnderscore,namespaceLowerCase,membersArg);
        {
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
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateSetMemberFull(i->first,i->second).c_str());
                fprintf(file.mFile, "    %s;\n",CreateGetMemberFull(i->first,i->second).c_str());
            }
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    friend class ComponentFactory;\n");
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateMemberWithType(i->first,i->second).c_str());
            }
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

            fprintf(file.mFile, "//TODO: to component_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateComponent<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to component_loader_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateComponentLoader<%sLoader>);\n",classUnderscore.c_str(),classCamelCase.c_str());
        }
        

        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"core/%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            bool isFirst=true;
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s %s(_fill_me_)\n",
                    isFirst?":":",",CreateMemberName(i->second).c_str());
                isFirst=false;
            }
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "%s",CreateSetMemberCppDefiniton(i->first,i->second,classCamelCase).c_str());
                fprintf(file.mFile, "%s",CreateGetMemberCppDefiniton(i->first,i->second,classCamelCase).c_str());
            }

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %sLoader::BindValues()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%sLoader::%sLoader()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
        }

        L1("ComponentGenerator ended\n");
    }
};

class NormalItemGenerator : public Generator
{
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        L1("NormalItemGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="NormalItem";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="core";
        }

        Init(classUnderscore,parentUnderscore,namespaceLowerCase,membersArg);
        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"normal_item.h\"\n");
            fprintf(file.mFile, "#include \"core/property_loader.h\"\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    %s( int32_t id );\n",classCamelCase.c_str());
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateSetMemberFull(i->first,i->second).c_str());
                fprintf(file.mFile, "    %s;\n",CreateGetMemberFull(i->first,i->second).c_str());
            }
            fprintf(file.mFile, "protected:\n");
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateMemberWithType(i->first,i->second).c_str());
            }
            fprintf(file.mFile, "private:\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to item_factory.cpp:\n");
            fprintf(file.mFile, "Bind<%s>(AutoId(\"%s\"));\n",classCamelCase.c_str(),classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"core/%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s( int32_t id )\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : NormalItem(id)\n");
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s %s(_fill_me_)\n",
                    ",",CreateMemberName(i->second).c_str());
            }
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "%s",CreateSetMemberCppDefiniton(i->first,i->second,classCamelCase).c_str());
                fprintf(file.mFile, "%s",CreateGetMemberCppDefiniton(i->first,i->second,classCamelCase).c_str());
            }

            fprintf(file.mFile, "\n");
        }

        L1("NormalItemGenerator ended\n");
    }
};


class SystemGenerator : public Generator
{
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        L1("SystemGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="system";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="engine";
        }

        Init(classUnderscore,parentUnderscore,namespaceLowerCase,membersArg);

        std::string targetComponentCamelCase="SomeTarget";
        std::string targetComponentVariableName="someTarget";
        std::string targetComponentUnderscore="some_target";
        for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
        {
            if(i->first=="component")
            {
                targetComponentUnderscore=i->second;
                targetComponentCamelCase=UnderscoreToCamelCase(targetComponentUnderscore);
                targetComponentVariableName=targetComponentCamelCase;
                targetComponentVariableName[0]=tolower(targetComponentVariableName[0]);
                break;
            }
        }
        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"core/scene.h\"\n");
            fprintf(file.mFile, "#include \"engine/%s.h\"\n",parentUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SYSTEM_BASE(%s)\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Update( double DeltaTime );\n");
            fprintf(file.mFile, "private:\n");
            fprintf(file.mFile, "    Scene& mScene;\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateSystem<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to main.cpp:\n");
            fprintf(file.mFile, "Eng.AddSystem(AutoId(\"%s\"));\n",classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"platform/i_platform.h\"\n");
            fprintf(file.mFile, "#include \"%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "#include \"core/i_%s_component.h\"\n",targetComponentUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : mScene( Scene::Get() )\n");
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Init()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Update(double DeltaTime)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "       Actor& actor = **it;\n");
            fprintf(file.mFile, "       Opt<I%sComponent> %sC=actor.Get<I%sComponent>();\n",targetComponentCamelCase.c_str(),targetComponentVariableName.c_str(),targetComponentCamelCase.c_str());
            fprintf(file.mFile, "       if (!%sC.IsValid())\n",targetComponentVariableName.c_str());
            fprintf(file.mFile, "       {\n");
            fprintf(file.mFile, "           continue;\n");
            fprintf(file.mFile, "       }\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }

        L1("SystemGenerator ended\n");
    }
};


class SubSystemGenerator : public Generator
{
protected:
    std::string targetComponentCamelCase;
    std::string targetComponentVariableName;
    std::string targetComponentUnderscore;

    virtual void Init(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        Generator::Init(classUnderscore,parentUnderscore,namespaceLowerCase,membersArg);
        targetComponentCamelCase="SomeTarget";
        targetComponentVariableName="someTarget";
        targetComponentUnderscore="some_target";
        for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
        {
            if(i->first=="component")
            {
                targetComponentUnderscore=i->second;
                targetComponentCamelCase=UnderscoreToCamelCase(targetComponentUnderscore);
                targetComponentVariableName=targetComponentCamelCase;
                targetComponentVariableName[0]=tolower(targetComponentVariableName[0]);
                break;
            }
        }
    }

    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        L1("SubSystemGenerator started\n");
        L1("SubSystemGenerator ended\n");
    }
};



class CollisionSubSystemGenerator : public SubSystemGenerator
{
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        L1("CollisionSubSystemGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="collision_sub_system";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="engine";
        }

        Init(classUnderscore,parentUnderscore,namespaceLowerCase,membersArg);

        std::string targetComponentCamelCase="SomeTarget";
        std::string targetComponentVariableName="someTarget";
        std::string targetComponentUnderscore="some_target";
        for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
        {
            if(i->first=="component")
            {
                targetComponentUnderscore=i->second;
                targetComponentCamelCase=UnderscoreToCamelCase(targetComponentUnderscore);
                targetComponentVariableName=targetComponentCamelCase;
                targetComponentVariableName[0]=tolower(targetComponentVariableName[0]);
                break;
            }
        }
        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"engine/collisions/collision_sub_system.h\"\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n");
            fprintf(file.mFile, "    virtual void ClipScene( Actor& actor );\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to sub_system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to main.cpp:\n");
            fprintf(file.mFile, "collisionSS->AddSubSystem(AutoId(\"%s_collision_component\"),AutoId(\"%s\"));\n",targetComponentUnderscore.c_str(),classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"platform/i_platform.h\"\n");
            fprintf(file.mFile, "#include \"engine/engine.h\"\n");
            fprintf(file.mFile, "#include \"engine/collisions/%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "#include \"core/%s_collision_component.h\"\n",targetComponentUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : CollisionSubSystem()\n");
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Init()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    if (!mOther)\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        return;\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "    Opt<%sCollisionComponent> %sCC=actor.Get<%sCollisionComponent>();\n",targetComponentCamelCase.c_str(),targetComponentVariableName.c_str(),targetComponentCamelCase.c_str());
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::ClipScene(Actor& actor)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    CollisionSubSystem::ClipScene(actor);\n");
            fprintf(file.mFile, "    Opt<%sCollisionComponent> %sCC=actor.Get<%sCollisionComponent>();\n",targetComponentCamelCase.c_str(),targetComponentVariableName.c_str(),targetComponentCamelCase.c_str());
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }

        L1("CollisionSubSystemGenerator ended\n");
    }
};

class ControllerSubSystemGenerator : public SubSystemGenerator
{
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        L1("ControllerSubSystemGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="sub_system";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="engine";
        }

        Init(classUnderscore,parentUnderscore,namespaceLowerCase,membersArg);

        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"engine/sub_system.h\"\n");
            fprintf(file.mFile, "#include \"core/scene.h\"\n");
            fprintf(file.mFile, "#include \"core/program_state.h\"\n");
            fprintf(file.mFile, "using core::ProgramState;\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n");
            fprintf(file.mFile, "private:\n");
            fprintf(file.mFile, "    Scene& mScene;\n");
            fprintf(file.mFile, "    ProgramState& mProgramState;\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to sub_system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to main.cpp:\n");
            fprintf(file.mFile, "controllserSystem->AddSubSystem(AutoId(\"%s_controller_component\"),AutoId(\"%s\"));\n",targetComponentUnderscore.c_str(),classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"platform/i_platform.h\"\n");
            fprintf(file.mFile, "#include \"engine/engine.h\"\n");
            fprintf(file.mFile, "#include \"engine/controllers/%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "#include \"core/%s_controller_component.h\"\n",targetComponentUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : mScene(Scene::Get())\n");
            fprintf(file.mFile, "    , mProgramState(core::ProgramState::Get())\n");
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Init()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    Opt<%sControllerComponent> %sCC=actor.Get<%sControllerComponent>();\n",targetComponentCamelCase.c_str(),targetComponentVariableName.c_str(),targetComponentCamelCase.c_str());
            fprintf(file.mFile, "    if (!%sCC.IsValid()||!%sCC->IsEnabled())\n",targetComponentVariableName.c_str(),targetComponentVariableName.c_str());
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        return;\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }

        L1("ControllerSubSystemGenerator ended\n");
    }
};

class NormalItemSubSystemGenerator : public Generator
{
    virtual void Generate(std::string classUnderscore,std::string parentUnderscore,std::string namespaceLowerCase,std::string membersArg)
    {
        L1("NormalItemSubSystemGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="sub_system";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="engine";
        }

        Init(classUnderscore,parentUnderscore,namespaceLowerCase,membersArg);

        std::string targetItemTypeCamelCase="NormalItem";
        std::string targetItemTypeVariableName="normalItem";
        std::string targetItemTypeUnderscore="normal_item";
        std::string targetItemNameCamelCase="ItemName";
        std::string targetItemNameVariableName="itemName";
        std::string targetItemNameUnderscore="item_name";
        for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
        {
            if(i->first=="type")
            {
                targetItemTypeUnderscore=i->second;
                targetItemTypeCamelCase=UnderscoreToCamelCase(targetItemTypeUnderscore);
                targetItemTypeVariableName=targetItemTypeCamelCase;
                targetItemTypeVariableName[0]=tolower(targetItemTypeVariableName[0]);
            }
            else if (i->first=="item")
            {
                targetItemNameUnderscore=i->second;
                targetItemNameCamelCase=UnderscoreToCamelCase(targetItemNameUnderscore);
                targetItemNameVariableName=targetItemNameCamelCase;
                targetItemNameVariableName[0]=tolower(targetItemNameVariableName[0]);
            }
        }
        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"engine/items/common_sub_system_includes.h\"\n");
            fprintf(file.mFile, "#include \"%s_sub_system.h\"\n",targetItemTypeUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n");
            fprintf(file.mFile, "private:\n");
            fprintf(file.mFile, "    Scene& mScene;\n");
            fprintf(file.mFile, "    Opt<%sSubSystem> m%sSubSystem;\n",targetItemTypeCamelCase.c_str(),targetItemTypeCamelCase.c_str());
            fprintf(file.mFile, "    ActorFactory& mActorFactory;\n");
            fprintf(file.mFile, "    int32_t mProjectileId;\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to sub_system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to main.cpp:\n");
            fprintf(file.mFile, "%sSS->AddSubSystem(AutoId(\"%s_%s\"),AutoId(\"%s\"));\n",targetItemTypeVariableName.c_str(),targetItemNameUnderscore.c_str(),targetItemTypeUnderscore.c_str(),classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"engine/items/%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "#include \"%s_sub_system.h\"\n",targetItemTypeUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : mScene(Scene::Get())\n");
            fprintf(file.mFile, "    , m%sSubSystem(%sSubSystem::Get())\n",targetItemTypeCamelCase.c_str(),targetItemTypeCamelCase.c_str());
            fprintf(file.mFile, "    , mActorFactory(ActorFactory::Get())\n");
            fprintf(file.mFile, "    , mProjectileId(AutoId(\"_fill_me_\"))\n");
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Init()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();\n");
            fprintf(file.mFile, "    Opt<%s> %s = inventoryC->GetSelected%s();\n",targetItemTypeCamelCase.c_str(),targetItemTypeVariableName.c_str(),targetItemTypeCamelCase.c_str());
            fprintf(file.mFile, "    if (%s->IsUse())\n",targetItemTypeVariableName.c_str());
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        //TODO: do some stuff with it\n");
            fprintf(file.mFile, "        %s->SetConsumed(true);\n",targetItemTypeVariableName.c_str());
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }

        L1("NormalItemSubSystemGenerator ended\n");
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
        Bind( AutoId( "normal_item" ), &CreateGenerator<NormalItemGenerator> );
        Bind( AutoId( "system" ), &CreateGenerator<SystemGenerator> );
        Bind( AutoId( "collision_sub_system" ), &CreateGenerator<CollisionSubSystemGenerator> );
        Bind( AutoId( "controller_sub_system" ), &CreateGenerator<ControllerSubSystemGenerator> );
        Bind( AutoId( "normal_item_sub_system" ), &CreateGenerator<NormalItemSubSystemGenerator> );
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
    std::string membersArg;
	desc.add_options()
        ("help", "produce help message")
		("-c", po::value<std::string>(&classUnderscore), "class_name_underscore")
		("-g", po::value<std::string>(&generatorType), "type of generator")
		("-p", po::value<std::string>(&parentUnderscore), "parent_name_underscore")
        ("-n", po::value<std::string>(&namespaceLowerCase), "namespacelowercase")
        ("-m", po::value<std::string>(&membersArg), "optional: members: \"double-radius int32_t-targetId\"")
		("generators:", 
        "\n*** default_generator ***\n does nothing\n" 
        "\n*** i_component ***\n class_name shall be in \"i_{the_name_underscore}_component\" format. generates a class_name_underscore.h with abstract member getters setters. guesses the parent to Component.\n" 
        "\n*** component ***\n class_name shall be in \"{the_name_underscore}_component\" format. generates a class_name_underscore.h and class_name_underscore.cpp with getters setters and member variables. guesses the parent to i_class_name_underscore if not set \n" 
        "\n*** system ***\n class_name shall be in \"{the_name_underscore}_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -m \"component-target_component_name_without_component\" (e.g. for drop_on_death_component: -m \"component-drop_on_death\")\n" 
        "\n*** collision_sub_system ***\n class_name shall be in \"{the_name_underscore}_collision_sub_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -m \"component-target_component_name_without_collision_component\" (e.g. for shot_collision_component -m \"component-shot\")\n" 
        "\n*** controller_sub_system ***\n class_name shall be in \"{the_name_underscore}_controller_sub_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -m \"component-target_component_name_without_controller_component\" (e.g. for random_controller_component -m \"component-random\")\n" 
        "\n*** normal_item ***\n class_name shall be in \"{the_name_underscore}_normal_item\" format. generates a class_name_underscore.h and class_name_underscore.cpp with getters setters and member variables. guesses the parent to normal_item if not set \n" 
        "\n*** normal_item_sub_system ***\n class_name shall be in \"{the_name_underscore}_normal_item_sub_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -m \"item-target_item_name_without_normal_item\" (e.g. for grenade_normal_item -m \"item-grenade\")\n" 
        //"\n\n\n"
           )
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

    GeneratorFactory::Get()(AutoId(generatorType))->Generate(classUnderscore,parentUnderscore,namespaceLowerCase, membersArg);

	return 0;
}