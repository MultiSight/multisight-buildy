
#ifndef __Config_h
#define __Config_h

#include "XSDK/XString.h"
#include "XSDK/XMemory.h"
#include "XSDK/XRef.h"
#include "XSDK/XHash.h"
#include <list>
#include <vector>

struct Component
{
    XSDK::XString name;
    XSDK::XString src;
    XSDK::XString path;
    XSDK::XString rev;
    XSDK::XString cleanbuild;
    std::list<XSDK::XString> tags;
};

class Config
{
public:
    Config( const XSDK::XString& configPath );
    virtual ~Config() throw();

    size_t GetNumComponents();
    struct Component GetComponent( size_t index );

    std::list<struct Component> GetAllComponents();
    std::list<struct Component> GetComponentsByTag( const XSDK::XString& key );

    void Write( const XSDK::XString& path );

private:
    XRef<XSDK::XMemory> _ReadFile( const XSDK::XString& path );

    XSDK::XString _configPath;
    std::vector<struct Component> _components;
    XSDK::XHash<XRef<std::list<struct Component> > > _tagMembers;
};

#endif
