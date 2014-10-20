
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
