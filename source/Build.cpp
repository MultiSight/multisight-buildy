
#include "Build.h"
#include "XSDK/XPath.h"

using namespace XSDK;
using namespace std;

void CleanBuild( XRef<Config> cfg, const XSDK::XString& tag, bool release, bool removeDevelArtifacts )
{
    int err = 0;
    if( removeDevelArtifacts )
    {
        printf("Removing devel_artifacts...");
        fflush(stdout);

        err = system( "rm -rf devel_artifacts" );
	if( err < 0 )
	  X_THROW(("Unable to remove devel_artifacts."));
    }

    list<struct Component> components=(tag.length()>0) ? cfg->GetMatchingComponents(tag) : cfg->GetAllComponents();

    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
      printf("%s\n",i->cleanbuild.c_str());
      fflush(stdout);
        err = system( XString::Format( "\"%s\" %s %s %s %s",
				       i->cleanbuild.c_str(),
				       i->name.c_str(),
                       (i->src.length() > 0) ? i->src.c_str() : "NO_SOURCE",
				       i->path.c_str(),
                                       (release)?"RELEASE":"DEBUG" ).c_str() );
	if( err < 0 )
	  X_THROW(("Build command failure."));
    }
}
