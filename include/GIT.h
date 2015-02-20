
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __Buildy__GIT_h
#define __Buildy__GIT_h

#include "XSDK/XString.h"
#include "Config.h"

void GITClone( XRef<Config> cfg, const XSDK::XString& tag, bool excludeBranch = false, bool excludeRev = false );

void GITPull( XRef<Config> cfg, const XSDK::XString& tag );

void GITStatus( XRef<Config> cfg, const XSDK::XString& tag, const XSDK::XString& configDir );

void GITCheckout( XRef<Config> cfg, const XSDK::XString& tag, const XSDK::XString& configDir, const XSDK::XString& branchName );

void BuildySnapshot( XRef<Config> cfg, const XSDK::XString& outputFilePath );

#endif
