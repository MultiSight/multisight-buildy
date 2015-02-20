
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef __buildy_Option_h
#define __buildy_Option_h

#include "XSDK/XString.h"
#include <list>

struct Option
{
    XSDK::XString opt;
    XSDK::XString arg;
};

std::list<struct Option> ParseOptions( int argc, char* argv[] );

bool CheckOption( const std::list<struct Option>& options, const XSDK::XString& opt, XSDK::XString& arg );

bool CheckOption( const std::list<struct Option>& options, const XSDK::XString& opt );

#endif
