
#ifndef __Buildy__GIT_h
#define __Buildy__GIT_h

#include "XSDK/XString.h"
#include "Config.h"

void GITClone( XRef<Config> cfg, const XSDK::XString& tag );

void GITPull( XRef<Config> cfg, const XSDK::XString& tag );

void GITStatus( XRef<Config> cfg, const XSDK::XString& tag );

void GITCheckout( XRef<Config> cfg, const XSDK::XString& tag );

void BuildySnapshot( XRef<Config> cfg, const XSDK::XString& outputFilePath );

#endif
