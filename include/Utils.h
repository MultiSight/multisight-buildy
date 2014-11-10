
#ifndef __Buildy_Utils_h
#define __Buildy_Utils_h

#include "XSDK/XString.h"
#include "XSDK/XMemory.h"
#include "XSDK/XRef.h"

XSDK::XString GetDirectoryFromURL( const XSDK::XString& url );

XSDK::XString ExecAndGetOutput( const XSDK::XString& cmd );

XRef<XSDK::XMemory> ReadFile( const XSDK::XString& path );
XSDK::XString ReadFileAsString( const XSDK::XString& path );

void WriteFileContents( const XSDK::XString& fileName, const XSDK::XString& fileContents );

#endif
