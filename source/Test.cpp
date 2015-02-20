
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "Test.h"
#include "XSDK/XPath.h"

using namespace XSDK;
using namespace std;

void Test( XRef<Config> cfg, const XSDK::XString& tag )
{
    XString configDir = cfg->GetConfigDir();

    list<struct Component> components=(tag.length()>0) ? cfg->GetMatchingComponents(tag) : cfg->GetAllComponents();

    int err = 0;
    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        if( i->cleantest.length() > 0 )
        {
            err = system( XString::Format( "\"%s%s%s\" %s %s %s",
                                           configDir.c_str(),
                                           PATH_SLASH,
                                           i->cleantest.c_str(),
                                           i->name.c_str(),
                                           (i->src.length() > 0) ? i->src.c_str() : "NO_SOURCE",
                                           i->path.c_str() ).c_str() );
            if( err != 0 )
                X_THROW(("Test failure."));
        }
    }
}
