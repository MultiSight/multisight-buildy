
#include "Config.h"
#include "Utils.h"

#include "XSDK/LargeFiles.h"
#include "XSDK/XPath.h"

using namespace XSDK;
using namespace std;

//[
//   {
//        "name": "x264",
//        "src": "https://github.com/MultiSight/x264",
//        "path": "x264",
//        "rev": "f8eb7e276646f473822ba00369660491676b4be4",
//        "branch": "2.0",
//        "tags": [ "3rdparty", "gateway_libs" ]
//    }
//],
//[
//    {
//        "name": "xsdk",
//        "src": "https://github.com/MultiSight/xsdk",
//        "path": "xsdk",
//        "tags": [ "gateway_libs" ]
//    }
//],


Config::Config( const XString& configDir, const XString& configFileName ) :
    _configDir( configDir ),
    _configPath( XString::Format( "%s%s%s", _configDir.c_str(), PATH_SLASH, (configFileName.length()!=0)?configFileName.c_str():"buildy.json" ) ),
    _localConfigPath( "local.json" ),
    _components(),
    _tagMembers()
{
    XIRef<XJSONItem> localComponents;

    if( XPath::Exists( _localConfigPath ) )
    {
        XRef<XMemory> localConfigBuffer = ReadFile( _localConfigPath );
        XString localDoc = XString( (char*)localConfigBuffer->Map(), localConfigBuffer->GetDataSize() );
        localComponents = XJSONItem::ParseDocument( localDoc )->Index( "components" );
    }

    XRef<XMemory> buffer = ReadFile( _configPath );
    XString doc = XString( (char*)buffer->Map(), buffer->GetDataSize() );
    XIRef<XJSONItem> components = XJSONItem::ParseDocument( doc )->Index( "components" );

    if (components.Get() == NULL)
        X_THROW(("Found no build components in config"));

    size_t numComponents = components->Count();

    size_t numLocalComponents = (localComponents.Get()) ? localComponents->Count() : 0;

    _components.reserve( numComponents + numLocalComponents );

    for( size_t i = 0; i < numComponents; i++ )
    {
        XIRef<XJSONItem> bc = components->Index( i );
        _components.push_back( _CreateComponent( bc ) );
    }

    for( size_t i = 0; i < numLocalComponents; i++ )
    {
        XIRef<XJSONItem> bc = localComponents->Index( i );
        _components.push_back( _CreateComponent( bc ) );
    }
}

Config::~Config() throw()
{
}

XString Config::GetConfigDir() const
{
    return _configDir;
}

size_t Config::GetNumComponents()
{
    return _components.size();
}

struct Component Config::GetComponent( size_t index )
{
    if( index >= _components.size() )
        X_THROW(("Invalid component index."));

    return _components[index];
}

list<struct Component> Config::GetAllComponents()
{
    list<struct Component> components;

    vector<struct Component>::iterator i;
    for( i = _components.begin(); i != _components.end(); i++ )
        components.push_back( *i );

    return components;
}

list<struct Component> Config::GetComponentsByTag( const XString& key )
{
    list<struct Component> components;

    if( _tagMembers.Find( key ) )
        components = *(_tagMembers[key]);

    return components;
}

list<struct Component> Config::GetMatchingComponents( const XString& arg )
{
    list<struct Component> matched = GetComponentsByTag( arg );

    vector<struct Component>::iterator i;
    for( i = _components.begin(); i != _components.end(); i++ )
        if( i->name == arg )
            matched.push_back( *i );

    return matched;
}

void Config::SetAllComponents( const list<struct Component>& components )
{
    _components.clear();

    list<struct Component>::const_iterator i;
    for( i = components.begin(); i != components.end(); i++ )
        _components.push_back( *i );
}

void Config::Write( const XString& path )
{
    XString doc;

    doc += "{\n";
    doc += "  \"components\":\n";
    doc += "  [\n";

    vector<struct Component>::iterator i;
    for( i = _components.begin(); i != _components.end(); i++ )
    {
        if( i->src.length() > 0 )
        {
            bool lastComponent = (next(i) == _components.end()) ? true : false;

            doc += "    {\n";
            doc += XString::Format( "      \"name\": \"%s\",\n", i->name.c_str() );
            doc += XString::Format( "      \"src\": \"%s\",\n", i->src.c_str() );
            doc += XString::Format( "      \"path\": \"%s\",\n", i->path.c_str() );
            if( i->rev.length() > 0 )
                doc += XString::Format( "      \"rev\": \"%s\",\n", i->rev.c_str() );
            if( i->branch.length() > 0 )
                doc += XString::Format( "      \"branch\": \"%s\",\n", i->branch.c_str() );
            if( i->tags.size() > 0 )
            {
                doc += "      \"tags\": [\n";

                list<XString>::iterator ii;
                for( ii = i->tags.begin(); ii != i->tags.end(); ii++ )
                {
                    bool lastTag = (next(ii) == i->tags.end()) ? true : false;
                    doc += XString::Format( "        \"%s\"%s\n", ii->c_str(), (lastTag) ? "" : "," );
                }

                doc += "      ],\n";
            }

            if( !i->cleanbuildContents.length() )
                X_THROW(("Snapshots require embedded build scripts."));

            doc += XString::Format( "      \"cleanbuild_contents\": \"%s\"\n",
                                    XString::Base64Encode( i->cleanbuildContents.c_str(), i->cleanbuildContents.length() ).c_str() );

            doc += XString::Format( "    }%s\n", (lastComponent) ? "" : "," );
        }
    }

    doc += "  ]\n";
    doc += "}\n";

    FILE* outputFile = fopen( path.c_str(), "wb" );
    if( !outputFile )
        X_THROW(("Unable to open output file."));

    fwrite( doc.c_str(), 1, doc.length(), outputFile );

    fclose( outputFile );
}

struct Component Config::_CreateComponent( XIRef<XJSONItem> bc )
{
    if( !bc->HasIndex( "name" ) )
        X_THROW(("Build component missing required: name"));
    if( !bc->HasIndex( "src" ) )
        X_THROW(("Build component missing required: src"));
    if( !bc->HasIndex( "path" ) )
        X_THROW(("Build component missing required: path"));

    struct Component component;

    component.name = bc->Index( "name" )->Get<XString>();
    component.src = bc->Index( "src" )->Get<XString>();
    component.path = bc->Index( "path" )->Get<XString>();
    component.rev = (bc->HasIndex( "rev" )) ? bc->Index( "rev" )->Get<XString>() : "";
    component.branch = (bc->HasIndex( "branch" )) ? bc->Index( "branch" )->Get<XString>() : "";
    component.cleanbuild = (bc->HasIndex( "cleanbuild" )) ? bc->Index( "cleanbuild" )->Get<XString>() : "";

    if( bc->HasIndex( "cleanbuild_contents" ) )
    {
        XString encodedCleanBuildContents = bc->Index( "cleanbuild_contents" )->Get<XString>();
        XIRef<XMemory> decoded = encodedCleanBuildContents.FromBase64();

        component.cleanbuildContents = XString( (char*)decoded->Map(), decoded->GetDataSize() );
    }

    if( bc->HasIndex( "tags" ) )
    {
        for( size_t ii = 0; ii < bc->Index( "tags" )->Count(); ii++ )
        {
            XString tag = bc->Index( "tags" )->Index( ii )->Get<XString>();

            component.tags.push_back( tag );

            if( _tagMembers.Find( tag ) == NULL )
                _tagMembers.Add( tag, new list<struct Component>() );

            _tagMembers[tag]->push_back( component );
        }
    }

    return component;
}
