
#include "Config.h"

#include "XSDK/XJSON.h"
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
    _components(),
    _tagMembers()
{
    XRef<XMemory> buffer = _ReadFile( _configPath );
    XString doc = XString( (char*)buffer->Map(), buffer->GetDataSize() );
    XIRef<XJSONItem> components = XJSONItem::ParseDocument( doc )->Index( "components" );

    _components.reserve( components->Count() );

    for( size_t i = 0; i < components->Count(); i++ )
    {
        XIRef<XJSONItem> bc = components->Index( i );

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
        component.cleanbuild = bc->Index( "cleanbuild" )->Get<XString>();

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

        _components.push_back( component );
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
        doc += XString::Format( "      \"cleanbuild\": \"%s\"\n", i->cleanbuild.c_str() );

        doc += XString::Format( "    }%s\n", (lastComponent) ? "" : "," );
    }

    doc += "  ]\n";
    doc += "}\n";

    FILE* outputFile = fopen( path.c_str(), "wb" );
    if( !outputFile )
        X_THROW(("Unable to open output file."));

    fwrite( doc.c_str(), 1, doc.length(), outputFile );

    fclose( outputFile );
}

XRef<XMemory> Config::_ReadFile( const XString& path )
{
    struct x_file_info fileInfo;
    if( x_stat( path, &fileInfo ) < 0 )
        X_THROW(("Unable to stat config."));

    FILE* inFile = fopen( path.c_str(), "r+b" );
    if( !inFile )
        X_THROW(("Unable to open: %s", path.c_str()));

    uint32_t fileSize = (uint32_t)fileInfo._fileSize;
    uint32_t blockSize = (uint32_t)fileInfo._optimalBlockSize;

    size_t numBlocks = (fileSize > blockSize) ? fileSize / blockSize : 0;
    size_t remainder = (fileSize >= blockSize) ? (fileSize % blockSize) : fileSize;

    XRef<XMemory> result = new XMemory;
    uint8_t* dst = &result->Extend( fileSize );

    while( numBlocks > 0 )
    {
        size_t itemsRead = fread( dst, blockSize, 1, inFile );
        if( itemsRead > 0 )
        {
            dst += (itemsRead * blockSize);
            numBlocks -= itemsRead;
        }
    }

    while( remainder > 0 )
    {
        size_t bytesRead = fread( dst, 1, remainder, inFile );
        if( bytesRead > 0 )
            remainder -= bytesRead;
    }

    return result;
}
