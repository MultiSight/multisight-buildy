
#include "XSDK/Types.h"
#include "XSDK/XPath.h"

#include "GIT.h"
#include "Utils.h"

using namespace XSDK;
using namespace std;

static bool _UncommittedOrUnstashedChanges( XRef<Config> cfg, const XString& tag = "" )
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

            if( output.length() > 0 )
            {
                printf( "Found uncommited or unstashed changes in %s.\n", i->name.c_str() );
                fflush( stdout );

                result = true;
                break;
            }
        }
    }

    return result;
}

void GITClone( XRef<Config> cfg, const XSDK::XString& tag, bool excludeBranch, bool excludeRev )
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

                if( !excludeBranch )
                {
                    // only do branch checkouts if revision is not set...
                    if( i->rev.length() == 0 )
                    {
                        if( i->branch.length() > 0 )
                        {
                            err = system( XString::Format( "git -C %s checkout %s",
                                                           dir.c_str(),
                                                           i->branch.c_str() ).c_str() );
                            if( err < 0 )
                                X_THROW(("Unable to execute git command."));
                        }
                    }
                }

                if( !excludeRev )
                {
                    if( i->rev.length() > 0 )
                    {
                        err = system( XString::Format( "git -C %s checkout %s",
                                                       dir.c_str(),
                                                       i->rev.c_str() ).c_str() );
                        if( err < 0 )
                            X_THROW(("Unable to execute git command."));
                    }
                }
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

void BuildySnapshot( XRef<Config> cfg, const XString& outputFilePath )
{
    if( _UncommittedOrUnstashedChanges( cfg ) )
        X_THROW(("Cannot snapshot with uncommited or unstashed changes."));

    list<struct Component> components = cfg->GetAllComponents();

    int err = 0;
    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        if( i->src.length() > 0 )
        {
            XString dir = GetDirectoryFromURL( i->src );

            i->branch = ExecAndGetOutput(XString::Format( "git -C %s rev-parse --abbrev-ref HEAD", dir.c_str() )).Strip();
            i->rev = ExecAndGetOutput(XString::Format( "git -C %s rev-parse HEAD", dir.c_str() )).Strip();
        }
    }

    cfg->SetAllComponents( components );

    cfg->Write( outputFilePath );
}

void GITCheckout( XRef<Config> cfg, const XString& tag )
{
#if 0
    if( _UncommittedOrUnstashedChanges( cfg, tag ) )
        X_THROW(("Cannot checkout with uncommited or unstashed changes."));

    list<struct Component> components=(tag.length()>0) ? cfg->GetMatchingComponents(tag) : cfg->GetAllComponents();

    int err = 0;
    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        if( i->src.length() > 0 )
        {
            XString dir = GetDirectoryFromURL( i->src );

//            err = system( XString::Format( "git -C %s checkout %s", dir.c_str(), rev ).c_str() );
//            if( err < 0 )
//                X_THROW(("Unable to execute git command."));
        }
    }
#endif
}
