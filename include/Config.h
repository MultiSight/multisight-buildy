
#ifndef __Config_h
#define __Config_h

#include "XSDK/XString.h"
#include "XSDK/XMemory.h"
#include "XSDK/XRef.h"
#include "XSDK/XHash.h"
#include "XSDK/XJSON.h"
#include <list>
#include <vector>

struct Component
{
    XSDK::XString name;
    XSDK::XString src;
    XSDK::XString path;
    XSDK::XString rev;
    XSDK::XString branch;
    XSDK::XString cleanbuild;
    XSDK::XString cleanbuildContents;
    std::list<XSDK::XString> tags;
};

class Config
{
public:
    Config( const XSDK::XString& configDir, const XSDK::XString& configFileName = "" );
    virtual ~Config() throw();

    XSDK::XString GetConfigDir() const;

    size_t GetNumComponents();
    struct Component GetComponent( size_t index );

    std::list<struct Component> GetAllComponents();
    std::list<struct Component> GetComponentsByTag( const XSDK::XString& key );
    std::list<struct Component> GetMatchingComponents( const XSDK::XString& arg );

    void SetAllComponents( const std::list<struct Component>& components );

    void Write( const XSDK::XString& path );

private:
    struct Component _CreateComponent( XIRef<XSDK::XJSONItem> bc );

    XSDK::XString _configDir;
    XSDK::XString _configPath;
    XSDK::XString _localConfigPath;
    std::vector<struct Component> _components;
    XSDK::XHash<XRef<std::list<struct Component> > > _tagMembers;
};

#endif
