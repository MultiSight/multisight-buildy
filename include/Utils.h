
#ifndef __Buildy_Utils_h
#define __Buildy_Utils_h

#include "XSDK/XString.h"

XSDK::XString GetDirectoryFromURL( const XSDK::XString& url );

XSDK::XString ExecAndGetOutput( const XSDK::XString& cmd );

#endif
