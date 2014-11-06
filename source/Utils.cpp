
#include "Utils.h"

using namespace XSDK;

XString GetDirectoryFromURL( const XString& url )
{
    size_t searchEnd = url.size() - 1;
    if( url.EndsWith( '/' ) )
        searchEnd--;

    XString dir = url.substr( url.rfind( '/', searchEnd ) + 1 );

    if( dir.EndsWith( '/' ) )
        dir = dir.substr( 0, dir.length() - 1 );

    return dir;
}

XString ExecAndGetOutput( const XString& cmd )
{
    FILE* output = NULL;

#ifdef IS_WINDOWS
    output = _popen( cmd.c_str(), "rb" );
#else
    output = popen( cmd.c_str(), "rb" );
#endif

    if( !output )
        X_THROW(("Unable to popen() cmd = %s",cmd.c_str()));

    XString result;

    while( !feof( output ) )
    {
        char line[4096];
        if( fgets( &line[0], 4096, output ) != NULL )
            result += line;
    }

#ifdef IS_WINDOWS
    _pclose( output );
#else
    pclose( output );
#endif

    return result;
}
