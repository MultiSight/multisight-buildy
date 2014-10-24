
#include "Build.h"
#include "XSDK/XPath.h"

using namespace XSDK;
using namespace std;

void CleanBuild( XRef<Config> cfg, const XSDK::XString& tag, bool removeDevelArtifacts )
{
    if( removeDevelArtifacts )
    {
        printf("Removing devel_artifacts...");
        fflush(stdout);

        system( "rm -rf devel_artifacts" );
    }

    list<struct Component> components=(tag.length()>0) ? cfg->GetComponentsByTag(tag) : cfg->GetAllComponents();

    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        system( XString::Format( "%s %s %s %s",
                                 i->cleanbuild.c_str(),
                                 i->name.c_str(),
                                 i->src.c_str(),
                                 i->path.c_str() ).c_str() );
    }
}
