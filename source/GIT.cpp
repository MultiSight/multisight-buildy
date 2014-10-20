
#include "XSDK/Types.h"
#include "XSDK/XPath.h"

#include "GIT.h"
#include "Utils.h"

using namespace XSDK;
using namespace std;

void GITClone( XRef<Config> cfg, const XSDK::XString& tag )
{
    list<struct Component> components=(tag.length()>0) ? cfg->GetComponentsByTag(tag) : cfg->GetAllComponents();

    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        XString dir = GetDirectoryFromURL( i->src );

        if( !XPath::Exists( dir ) )
        {
            printf("cloning component: %s\n",i->name.c_str());
            fflush(stdout);

            system( XString::Format( "git clone %s %s", i->src.c_str(), dir.c_str() ).c_str() );
        }
        else
        {
            printf("component exits: %s\n",i->name.c_str());
            fflush(stdout);
        }
    }
}

void GITPull( XRef<Config> cfg, const XSDK::XString& tag )
{
    list<struct Component> components=(tag.length()>0) ? cfg->GetComponentsByTag(tag) : cfg->GetAllComponents();

    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        XString dir = GetDirectoryFromURL( i->src );

        printf("pulling: %s\n",dir.c_str());
        fflush(stdout);

        system( XString::Format( "git -C %s pull --rebase", dir.c_str() ).c_str() );
    }
}

void GITStatus( XRef<Config> cfg, const XSDK::XString& tag )
{
    list<struct Component> components=(tag.length()>0) ? cfg->GetComponentsByTag(tag) : cfg->GetAllComponents();

    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        XString dir = GetDirectoryFromURL( i->src );

        printf("status: %s\n",dir.c_str());
        fflush(stdout);

        system( XString::Format( "git -C %s status", dir.c_str() ).c_str() );
    }
}
