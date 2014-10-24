
#ifndef __Buildy_Build_h
#define __Buildy_Build_h

#include "XSDK/XRef.h"
#include "XSDK/XString.h"
#include "Config.h"

void CleanBuild( XRef<Config> cfg, const XSDK::XString& tag, bool removeDevelArtifacts = false );

#endif
