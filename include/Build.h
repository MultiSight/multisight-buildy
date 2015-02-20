
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __Buildy_Build_h
#define __Buildy_Build_h

#include "XSDK/XRef.h"
#include "XSDK/XString.h"
#include "Config.h"

void SuperClean( XRef<Config> cfg, const XSDK::XString& tag );

void CleanBuild( XRef<Config> cfg, const XSDK::XString& tag, bool release = false, bool removeDevelArtifacts = false );

#endif
