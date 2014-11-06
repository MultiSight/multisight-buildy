
#include "XSDK/Types.h"
#include "XSDK/XPath.h"

#include "GIT.h"
#include "Utils.h"

using namespace XSDK;
using namespace std;

static bool _UncommittedOrUnstashedChanges( XRef<Config> cfg, const XString& tag )
{
    bool result = false;

    list<struct Component> components=(tag.length()>0) ? cfg->GetMatchingComponents(tag) : cfg->GetAllComponents();

    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        if( i->src.length() > 0 )
        {
            XString dir = GetDirectoryFromURL( i->src );

            XString output = ExecAndGetOutput( XString::Format( "git -C %s status --short", dir.c_str() ) );

            printf( "output.length() = %d\n",output.length() );
            fflush(stdout);

            if( output.length() > 0 )
                result = true;
        }
    }

    return result;
}

void GITClone( XRef<Config> cfg, const XSDK::XString& tag )
{
    list<struct Component> components=(tag.length()>0) ? cfg->GetMatchingComponents(tag) : cfg->GetAllComponents();

    int err = 0;
    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        if( i->src.length() > 0 )
        {
            XString dir = GetDirectoryFromURL( i->src );

            if( !XPath::Exists( dir ) )
            {
                printf("cloning component: %s\n",i->name.c_str());
                fflush(stdout);

                err = system( XString::Format( "git clone %s %s", i->src.c_str(), dir.c_str() ).c_str() );
                if( err < 0 )
                    X_THROW(("Unable to execute git command."));
            }
            else
            {
                printf("component exits: %s\n",i->name.c_str());
                fflush(stdout);
            }
        }
    }
}

void GITPull( XRef<Config> cfg, const XSDK::XString& tag )
{
    list<struct Component> components=(tag.length()>0) ? cfg->GetMatchingComponents(tag) : cfg->GetAllComponents();

    int err = 0;
    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        if( i->src.length() )
        {
            XString dir = GetDirectoryFromURL( i->src );

            printf("pulling: %s\n",dir.c_str());
            fflush(stdout);

            err = system( XString::Format( "git -C %s pull --rebase", dir.c_str() ).c_str() );
            if( err < 0 )
                X_THROW(("Unable to execute git command."));
        }
    }
}

void GITStatus( XRef<Config> cfg, const XSDK::XString& tag )
{
    list<struct Component> components=(tag.length()>0) ? cfg->GetMatchingComponents(tag) : cfg->GetAllComponents();

    int err = 0;
    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        if( i->src.length() > 0 )
        {
            XString dir = GetDirectoryFromURL( i->src );

            printf("status: %s\n",i->path.c_str());
            fflush(stdout);

            err = system( XString::Format( "git -C %s status --short", dir.c_str() ).c_str() );
            if( err < 0 )
                X_THROW(("Unable to execute git command."));
        }
    }
}

void GITCheckout( XRef<Config> cfg, const XString& tag, const XString& rev )
{
    if( _UncommittedOrUnstashedChanges( cfg, tag ) )
    {
        printf("Uncommited or unstashed changes.\n");
        fflush(stdout);
    }
    else
    {
        printf("NO uncommited or unstashed changes.\n");
        fflush(stdout);
    }
}
