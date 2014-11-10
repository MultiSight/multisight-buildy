
#include "Utils.h"
#include "XSDK/LargeFiles.h"

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
    output = popen( cmd.c_str(), "r" );
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

XRef<XMemory> ReadFile( const XString& path )
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

XString ReadFileAsString( const XString& path )
{
    XRef<XMemory> buffer = ReadFile( path );
    return XString( (char*)buffer->Map(), buffer->GetDataSize() );
}

void WriteFileContents( const XSDK::XString& fileName, const XSDK::XString& fileContents )
{
    FILE* outFile = fopen( fileName.c_str(), "wb" );
    if( !outFile )
        X_THROW(("Unable to open output file."));

    fwrite( fileContents.c_str(), 1, fileContents.length(), outFile );

    fclose( outFile );
}
